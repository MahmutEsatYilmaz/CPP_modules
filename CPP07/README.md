# CPP07 — C++ Templates

Bu modülün amacı **template (şablon)** kavramını öğretmek: aynı kodu her
tip için tek tek yeniden yazmadan, **derleme zamanında** (compile-time)
tipe göre çoğaltılan tek bir kod kalıbı yazmak. CPP00-CPP06'da öğrenilen
her şey (OCF, exception, inheritance, polymorphism) burada da geçerli,
ama artık üstüne "tip de bir parametre" fikri ekleniyor.

---

## 0. Önce genel kavramlar

### Template nedir, neden var?

CPP05'te `Bureaucrat`, `Form` gibi somut sınıflar yazdık — her biri
belirli bir tipe (örn. `int grade`, `std::string name`) bağlıydı. Peki
`swap(int&, int&)` yazdıktan sonra `swap(std::string&, std::string&)`,
`swap(double&, double&)` de gerekiyorsa? Fonksiyonun **gövdesi birebir
aynı** (`T tmp = a; a = b; b = tmp;`), sadece tip değişiyor. Bunu elle
her tip için kopyalamak yerine, tipi de bir **parametre** haline
getiriyoruz:

```cpp
template <typename T>
void swap(T &a, T &b) { T tmp = a; a = b; b = tmp; }
```

`swap(a, b)` çağrıldığında derleyici `a`/`b`'nin gerçek tipine bakıp
(örn. `int`) o tip için `swap<int>`'i **kendisi üretir** — buna
**template instantiation** (şablon somutlaştırma) denir. Bu tamamen
derleme zamanında olur; çalışma zamanında (runtime) hiçbir ekstra
maliyet yoktur (virtual fonksiyon çağrısındaki gibi bir "hangi
fonksiyon" araması yok).

### Compile-time genericity vs. runtime polymorphism

CPP05/CPP06'da öğrenilen **polymorphism** (örn. `AForm*` üzerinden
`execute()` çağırmak) da "aynı kodu farklı tipler için kullanma"
problemine bir çözümdü, ama **tamamen farklı bir mekanizmayla**:

| | Polymorphism (virtual) | Template (generic) |
|---|---|---|
| Ne zaman karar verilir | **Çalışma zamanında** (runtime, `vtable` üzerinden) | **Derleme zamanında** (compile-time) |
| Tipler arasındaki ilişki | Ortak bir **base class**'tan türemeleri gerekir (`AForm`) | Hiçbir ilişkiye gerek yok — `int`, `std::string`, kendi yazdığınız bir sınıf, hepsi olabilir |
| Maliyet | Küçük bir runtime maliyeti var (vtable lookup) | Runtime maliyeti **yok**, ama her farklı tip için ayrı bir kopya kod üretilir (binary boyutu artabilir) |
| Ne zaman kullanılır | "Bu nesnelerin **ortak bir arayüzü** var, hangi somut tip olduğunu çalışma zamanında bilmem gerekmiyor" (örn. `ShrubberyCreationForm` mü `RobotomyRequestForm` mü — `AForm` referansı üzerinden fark etmiyor) | "Bu algoritma **herhangi bir tiple** çalışır, aralarında hiçbir ortak base class ilişkisi yok" (`int` ile `std::string`'in ortak bir base class'ı yok, ama ikisi de `<`, `>`, `=` destekliyor) |

Özetle: polymorphism "farklı tipler, ortak arayüz, runtime'da seçim";
template "farklı tipler, ortak **söz dizimsel davranış** (örn. `<`
operatörünü destekliyor olmaları), compile-time'da çoğaltma". Bu modül
templateları öğretiyor çünkü ikisi **birbirinin yerine geçmez**, farklı
problemlere çözüm.

### Templatelar neden header'da tanımlanmak zorunda?

Bu modülün en önemli pratik kuralı: template fonksiyon/sınıf
**gövdeleri** `.cpp` dosyasına konulamaz, header'da (`.hpp`) ya da
header'ın include ettiği bir `.tpp`'de olmak zorunda. Nedeni **One
Definition Rule (ODR)** ve derleyicinin çalışma şekliyle ilgili:

- Normal bir fonksiyon (`int foo();`) `.cpp`'de **bir kez** derlenir,
  linker onu kullanan her yere bağlar.
- Ama bir template, `foo<int>` gibi somut bir tip verilmeden **derlenebilir
  kod üretemez** — `template <typename T> void swap(T&, T&)` başlı
  başına bir fonksiyon değil, bir fonksiyon **kalıbıdır**. Derleyici
  `swap(a, b)` çağrısını gördüğü **her `.cpp` dosyasında**, o çağrıya
  göre (`a`'nın tipine göre) ilgili somutlaştırmayı **o an, o dosyada**
  üretmek zorundadır.
- Eğer template'in gövdesi sadece `whatever.cpp`'de olsaydı, o gövdeyi
  sadece `whatever.cpp`'yi derleyen çeviri birimi (translation unit)
  görebilirdi. `main.cpp` `swap(a, b)` çağırdığında, `main.cpp`'yi
  derleyen aşamada compiler'ın gövdeye erişimi olmazdı → **linker
  hatası** (`undefined reference`).
- Çözüm: gövdeyi `#include` edilen bir header'a koymak. Böylece
  `whatever.hpp`'yi include eden **her** `.cpp` dosyası, template'in tam
  tanımını görür ve kendi ihtiyaç duyduğu somutlaştırmayı kendi başına
  üretebilir. (Farklı `.cpp` dosyalarının aynı somutlaştırmayı
  üretmesi de bir sorun değil — derleyici/linker bunu ODR'nin template'e
  özel istisnasıyla tekilleştirir, birden fazla kopyası olsa bile
  hatayla sonuçlanmaz.)

Bu yüzden bu modülde "fonksiyon gövdeleri header'da yasak" kuralına
**templateler istisna** — aksine, templateler için header'da olmak
**zorunlu**.

---

## 1. ex00 — `swap` / `min` / `max` (`whatever.hpp`)

**Amaç:** En basit haliyle function template yazmayı öğretmek.

```cpp
template <typename T>
void swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template <typename T>
T const &min(T const &a, T const &b)
{
	return (a < b) ? a : b;
}

template <typename T>
T const &max(T const &a, T const &b)
{
	return (a > b) ? a : b;
}
```

- `swap`: parametreleri **referans** (`T&`) alıyor, çünkü değerleri
  yerinde değiştirmesi gerekiyor (değere göre alsaydı, sadece kopyaları
  değişirdi, çağıran taraftaki gerçek değişkenler etkilenmezdi).
- `min`/`max`: parametreleri `T const&` alıp `T const&` döndürüyor.
  Referans döndürmek (`T` yerine) gereksiz bir kopya oluşturmaktan
  kaçınıyor — özellikle `std::string` gibi büyük nesnelerde önemli.
  `const` hem parametrede hem dönüş tipinde: fonksiyon kendisi
  parametreleri değiştirmiyor, dönen referans üzerinden de dışarıdan
  değiştirilmesin diye.
- **Eşitlik durumunda "ikinciyi döndür" kuralı (tie-break)**: Subject
  açıkça istiyor ki `a == b` ise `min` **`b`**'yi, `max` **`b`**'yi
  döndürsün. Bunu ayrı bir `if (a == b) return b;` satırıyla değil, tek
  bir karşılaştırmayla elde ediyoruz:
  - `min`: `(a < b) ? a : b` — eğer `a < b` **doğruysa** `a` küçüktür,
    onu döndür. **Yanlışsa** (yani `a >= b`, bu hem `a > b` hem `a == b`
    durumunu kapsar) `b`'yi döndür. `a == b` durumunda `a < b` yanlış
    olduğu için otomatik olarak `b` döner — tam istenen davranış, ekstra
    kod yazmadan.
  - `max`: aynı mantığın simetriği — `(a > b) ? a : b`. `a == b`
    durumunda `a > b` yanlış olur, `b` döner.
  - Bu sayede sadece `operator<` ve `operator>` yeterli oluyor,
    `operator==`'a hiç ihtiyaç yok — subject'in "tüm karşılaştırma
    operatörlerini destekleyen herhangi bir tip" şartını en az
    operatör kullanarak karşılıyoruz.

### `main.cpp`

Subject'in verdiği örnek **birebir** kullanılıyor: önce `int a=2, b=3`
üzerinde `swap`/`min`/`max`, sonra `std::string c="chaine1", d="chaine2"`
üzerinde aynısı. Bu, templatein **tek bir kod kalıbıyla** hem `int` hem
`std::string` için çalıştığını kanıtlıyor — `min`/`max` `std::string`
için `operator<`/`operator>`'ı (sözlük sırası karşılaştırması) otomatik
kullanıyor, biz hiçbir şey özelleştirmedik.

Çıktı (`make re` + çalıştırma ile doğrulandı):
```
a = 3, b = 2
min( a, b ) = 2
max( a, b ) = 3
c = chaine2, d = chaine1
min( c, d ) = chaine1
max( c, d ) = chaine2
```

---

## 2. ex01 — `iter` (`iter.hpp`)

**Amaç:** Bir template fonksiyonun **birden fazla tip parametresi**
alabileceğini (burada hem dizi elemanının tipi `T`, hem çağrılacak
fonksiyonun tipi `F`) ve **const-correctness**'in template'lerle nasıl
"bedava" geldiğini öğretmek.

```cpp
template <typename T, typename F>
void iter(T *array, size_t const len, F f)
{
	for (size_t i = 0; i < len; ++i)
		f(array[i]);
}
```

- `T`: dizinin eleman tipi (`int`, `const int`, `std::string`, ne
  olursa). `T *array` parametresi sayesinde dizi ne verilirse `T`
  otomatik ondan **çıkarsanır (deduce edilir)**.
- `F`: çağrılacak fonksiyonun/fonksiyon nesnesinin tipi. `f(array[i])`
  şeklinde çağrılabilen her şey olabilir — sıradan bir fonksiyon, bir
  fonksiyon pointer'ı, ya da (C++11 öncesinde bu modülde kullanılmasa
  da teorik olarak) `operator()` tanımlı bir sınıf.
- Fonksiyon **başına** kayan bir index (`for` döngüsü) ile her
  elemanda `f` çağrılıyor, dönüş değeri yok (subject "returns nothing"
  diyor).

### Const/non-const ayrımı nasıl "otomatik" çözülüyor?

Subject özellikle "hem const hem non-const elemanları nasıl
destekleyeceğinizi iyi düşünün" diyor. Naif bir çözüm iki ayrı
overload yazmak olurdu (`iter(T*, ...)` ve `iter(const T*, ...)`), ama
buna hiç gerek yok — **tek bir template zaten ikisini de karşılıyor**:

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int const carr[5] = {10, 20, 30, 40, 50};

iter(arr, 5, doubleInt);   // T = int        → f, int&        alabilir
iter(carr, 5, print<int>); // T = const int  → f, const int&   almak ZORUNDA
```

- `iter(carr, ...)` çağrıldığında dizi tipi `const int[5]` olduğu için
  derleyici `T`'yi **`const int`** olarak çıkarsar (`T` **kendisi**
  `const`'u içeriyor, `const T*` yazmadık ama sonuç aynı). Bu durumda
  `T *array` parametresi `const int *array` olur.
- `array[i]` ifadesinin tipi de `const int&` olur (T=const int
  olduğundan). Eğer `f`'ye verilen fonksiyon elemanı **değiştirmeye**
  çalışan bir imza kullansaydı (örn. `void f(int&)`), `const int&`'i
  `int&`'e bağlamaya çalışırken **derleme hatası** alırdık — yani
  const'luk ihlali derleyici tarafından **otomatik olarak** yakalanır,
  bizim ekstra bir kontrol yazmamıza gerek yok.
- `iter(arr, ...)` çağrıldığında (`arr` non-const) `T = int` olur,
  `array[i]` tipi `int&`'dir; bu durumda hem `void f(int&)` (mutasyon
  yapan) hem `void f(int const&)` (sadece okuyan) fonksiyonları kabul
  eder — `int&`, `int const&`'e problemsiz bağlanır.

Yani "const/non-const desteği" için **tek bir template**, C++'ın kendi
tip çıkarımı (**template argument deduction**) ve referans bağlama
kurallarından bedava geliyor; iki ayrı fonksiyon yazmak hem gereksiz
kod tekrarı olurdu hem de subject'in aradığı asıl içgörüyü (deduction'ın
gücü) kaçırırdı.

### `main.cpp` testleri

- Non-const `int` dizisi → `print<int>` ile ekrana basılıyor.
- Aynı dizi → `doubleInt` (`void doubleInt(int&)`) ile **yerinde
  ikiye katlanıyor**, sonra tekrar basılarak mutasyonun çalıştığı
  gösteriliyor.
- `const int` dizisi → yine `print<int>` (`T const&` alan bir template)
  ile basılıyor; bu satırın derlenmesi zaten "const destekleniyor"
  kanıtı.
- `std::string` dizisi → `iter`'in gerçekten **herhangi bir tiple**
  çalıştığını (sadece `int` özel bir durum değil) göstermek için.

---

## 3. ex02 — `Array<T>` (`Array.hpp` + `Array.tpp`)

**Amaç:** Bir **class template** yazmayı, **deep copy** (derin kopya)
kavramını ve template üyelerinin `.tpp`'de nasıl organize edildiğini
öğretmek.

### Neden `Array.hpp` + `Array.tpp` ayrımı?

`Array.hpp` sadece sınıfın **arayüzünü** (üye değişkenler, fonksiyon
imzaları) tanımlıyor; gerçek gövdeler `Array.tpp`'de. `Array.hpp`
sonunda `#include "Array.tpp"` var — yani sonuçta ikisi tek bir header
gibi davranıyor (`.tpp` derlenmiyor, sadece include ediliyor), ama bu
ayrım **okunabilirlik** için: bir sınıfın "ne yaptığını" görmek isteyen
biri sadece `.hpp`'ye bakar, "nasıl yaptığını" merak eden `.tpp`'ye
iner — normal bir `.hpp`/`.cpp` ayrımının template dünyasındaki
karşılığı.

### Deep copy (derin kopya) vs. shallow copy (sığ kopya)

```cpp
private:
	T *_data;
	unsigned int _size;
```

`Array` içeride `new[]` ile ayrılmış **ham bir pointer** (`_data`)
tutuyor. Eğer copy constructor/`operator=` **yazılmasaydı**,
derleyicinin ürettiği **varsayılan** kopyalama sadece pointer'ı
kopyalardı (shallow copy):

```cpp
Array<int> a(5);
Array<int> b(a);   // varsayılan (yazılmamış) copy ctor olsaydı:
                    // b._data = a._data;  (AYNI belleği gösterirler!)
b[0] = 999;         // a[0] de 999 olurdu — istenmeyen paylaşım
// ~Array() b için çalışınca delete[] b._data;
// ~Array() a için çalışınca delete[] a._data; ← AYNI belleği iki kez delete! (UB, çökme)
```

Bu hem "kopyayı değiştirince orijinal de değişiyor" mantık hatasına
hem de **double free** (aynı belleği iki kez `delete`) çökmesine yol
açardı. Çözüm, **deep copy**: yeni bir bellek bloğu `new[]` ile ayırıp,
elemanları **tek tek** eski bloktan yeni bloğa kopyalamak:

```cpp
Array<T>::Array(Array const &other)
	: _data(new T[other._size]), _size(other._size)
{
	for (unsigned int i = 0; i < _size; ++i)
		_data[i] = other._data[i];
}
```

Artık `a` ve `b` **tamamen ayrı** bellek bloklarına sahip; birini
değiştirmek diğerini etkilemez, her ikisinin destructor'ı kendi
bloğunu `delete[]` eder, çakışma olmaz. `operator=` de aynı mantıkla
yazıldı, tek fark: önce **yeni** bloğu ayırıp elemanları kopyalıyor,
sonra **eski** `_data`'yı `delete[]` ediyor (kendi kendine atama
`a = a;` durumunda önce eskiyi silip sonra ondan kopyalamaya
çalışmamak için — `if (this == &other) return *this;` ile ayrıca da
korunuyor).

### `new T[n]()` neden `new T[n]` değil?

Subject'in verdiği ipucu: `int *a = new int(); *a` bastırıldığında
`0` çıkar, çöp değer değil. Bunun sebebi `new T()` (parantezli) sözdiziminin
**value-initialization** yapması: `T` bir built-in tip (`int`, `double`...)
ise `0`/`0.0` ile başlatılır; `T` bir sınıf ise o sınıfın default
constructor'ı çağrılır. `new T[n]` (parantezsiz) ise **default-initialization**
yapar — built-in tipler için **hiçbir başlatma yapılmaz**, bellek
çöp değerlerle kalır. `new T[n]()` (dizi + parantez) bu value-init
davranışını **dizinin her elemanına** uygular:

```cpp
Array<T>::Array(unsigned int n) : _data(new T[n]()), _size(n) { }
```

Bu sayede `Array<int> a(5)` sonrası her `a[i]` garanti olarak `0`'dır
— subject'in test etmenizi istediği tam olarak bu.

### `new[]` zorunluluğu ve "önceden fazla ayırma" yasağı

Subject `new[]` kullanmayı zorunlu tutuyor (bu bir class template
projesinde beklenen şey zaten) ve **"preventive"** (ileriye dönük,
olası büyümeye karşı fazladan) ayırma yapmayı yasaklıyor — yani `Array`
kendi içinde "belki büyür" diye `capacity` gibi bir kavramla gerçek
`_size`'dan daha büyük bir blok ayıramaz. Bu implementasyon bunu zaten
doğal olarak sağlıyor: her yapıcı/atama tam olarak **ihtiyaç duyulan**
eleman sayısı kadar (`new T[n]`, `new T[other._size]`) ayırıyor, ne
eksik ne fazla; `operator[]` sınır kontrolüyle bu ayrılan alanın
dışına asla erişilmiyor.

### `operator[]` ve sınır (bounds) kontrolü

```cpp
T &Array<T>::operator[](unsigned int idx)
{
	if (idx >= _size)
		throw std::out_of_range("Array: index out of bounds");
	return _data[idx];
}
```

- İki overload var: **mutable** (`T&` döndüren, `const`
  olmayan nesneler için) ve **const** (`T const&` döndüren, `const
  Array<T>&` üzerinden çağrılabilen). İkisi de gerekli çünkü `main.cpp`'de
  `Array<int> const &constRef = a;` gibi bir const referans üzerinden de
  `operator[]` çağrılabiliyor olmalı — sadece mutable overload olsaydı bu
  derlenmezdi (const nesne üzerinden const olmayan bir üye fonksiyon
  çağrılamaz).
- `idx >= _size` kontrolü **her iki overload'da da** var (kod
  tekrarı gibi görünse de, template context'te bu iki fonksiyonu tek bir
  yardımcıya indirgemek subject'in istediği basit sınıf tasarımının
  ötesine geçerdi — burada okunabilirlik tercih edildi).
  `unsigned int idx` olduğu için negatif index derleyici seviyesinde
  zaten imkânsız (negatif bir değer verilirse `unsigned`'a
  dönüşürken dev bir pozitif sayıya sarar — o durum da `idx >= _size`
  ile zaten yakalanır).
- `std::out_of_range`, `<stdexcept>`'ten geliyor ve `std::exception`'dan
  türüyor — subject STL **container**'larını (`vector`, `map`...) ve
  `<algorithm>`'i yasaklıyor, ama `<stdexcept>` bunlardan biri değil,
  sadece birkaç exception sınıfı tanımlayan bir header, o yüzden
  kullanımı serbest.

### `size() const`

```cpp
unsigned int Array<T>::size() const;
```

Subject'in şartı: parametre almayacak ve nesneyi değiştirmeyecek. Bu
sonundaki `const` ile derleyiciye garanti ediliyor — fonksiyon gövdesi
içinde `_data`/`_size`'a yazma denemesi (kazayla bile olsa) derleme
hatası verir. Bu, `main.cpp`'de bir `Array<int> const&` üzerinden de
`size()`'ın çağrılabilmesini sağlıyor (const-correctness zinciri:
`operator[] const` ile aynı gerekçe).

### `main.cpp` testleri

- Default constructor ile boş dizi + `size()==0` kontrolü.
- `Array<int> a(5)` ile tüm elemanların `0` olduğunun gösterilmesi
  (subject'in ipucunu doğrudan doğrulayan test).
- `operator[]` ile elemanları değiştirme.
- Copy constructor (`Array<int> b(a)`) ve copy assignment
  (`Array<int> c; c = a;`) ile, kopya değiştirildiğinde **orijinalin
  etkilenmediğinin** (deep copy kanıtı) gösterilmesi.
- Hem mutable hem `const` referans (`Array<int> const &constRef`)
  üzerinden sınır dışı erişim denenip `try/catch` ile yakalanması.
- Son olarak dört farklı `Array`'in (`empty`, `a`, `b`, `c`) `size()`
  değerlerinin hepsinin beklenen ve birbirinden bağımsız olduğunun
  teyidi.

---

## 4. Bu modülde öğrenilen kavramların özeti

- **Function template**: `template <typename T>` ile tipten bağımsız,
  derleme zamanında tipe özel kod üreten fonksiyonlar yazmak
  (`swap`/`min`/`max`).
- **Template argument deduction**: Fonksiyon çağrısındaki gerçek
  argümanlardan (`iter(carr, ...)`'ta `T=const int` gibi) tip
  parametresinin derleyici tarafından otomatik çıkarsanması.
- **Templateler neden header'da yaşamalı**: ODR ve "her çeviri
  birimi, kullandığı somutlaştırmayı kendisi üretmek zorunda" kuralı
  yüzünden gövde `.cpp`'de değil `.hpp`/`.tpp`'de olmalı.
- **Compile-time genericity vs. runtime polymorphism**: İki farklı
  "aynı kodu farklı tiplerle kullanma" stratejisi — biri (virtual)
  ortak base class + runtime seçim, diğeri (template) hiçbir ilişki
  gerektirmeden compile-time çoğaltma.
- **Çoklu tip parametreli template'ler** (`template <typename T,
  typename F>`): Bir template'in birden fazla bağımsız tip parametresi
  alabilmesi (`iter`'daki dizi elemanı tipi ve fonksiyon tipi).
- **Const-correctness'in deduction ile otomatik gelmesi**: `const
  T*` yazmadan, sadece `T*` ile hem const hem non-const dizileri
  desteklemek — const'luk ihlalinin ayrı kod yazmadan derleyici
  tarafından reddedilmesi.
- **Class template**: `template <typename T> class Array { ... };` ile
  herhangi bir eleman tipiyle çalışan genel bir konteyner sınıfı
  yazmak.
- **Deep copy vs. shallow copy**: Ham pointer tutan bir sınıfta,
  varsayılan (derleyici üretimi) kopyalamanın neden tehlikeli
  (paylaşılan bellek, double free) olduğu ve elemanları tek tek
  kopyalayarak nasıl güvenli hale getirildiği.
- **Value-initialization (`new T[n]()`)**: Parantezli `new` ile
  built-in tiplerin çöp değil `0`/`0.0` ile başlatılması.
- **`new[]`/`delete[]` disiplinli kullanımı**: Tam ihtiyaç kadar
  ayırma (preventive allocation yasağı), her `new[]`'in bir
  `delete[]`'le eşleşmesi.
- **Exception ile sınır kontrolü**: STL container'ları olmadan da
  `<stdexcept>` ile güvenli, exception fırlatan bir `operator[]`
  yazılabileceği.
- **`const` overload çiftleri** (`operator[]` hem mutable hem const):
  Bir üye fonksiyonun hem `const` hem non-const nesneler için ayrı ayrı
  sağlanması gerektiği, aksi halde const nesnelerin o fonksiyonu hiç
  çağıramayacağı.
