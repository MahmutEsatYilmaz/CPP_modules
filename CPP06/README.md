# CPP06 — C++ Casts

Bu modülün amacı **C++'ın cast (tür dönüştürme) mekanizmalarını** öğretmek:
`static_cast`, `reinterpret_cast`, `dynamic_cast` (ve dolaylı olarak
`const_cast`'in neden burada kullanılmadığı). C'de tek bir "evrensel" cast
sözdizimi (`(int)x`) vardı; C++ bunu kasıtlı olarak dörde bölüyor çünkü her
biri **farklı bir riski ve farklı bir garantiyi** temsil ediyor. Modülün
"Additional Rule"ü tam olarak bunu test ediyor: her egzersizde **o
egzersize özgü, doğru** cast'i seçip kullanmak — her yere `static_cast`
serpiştirmek değil.

---

## 0. Önce genel kavram: neden 4 farklı cast var?

| Cast | Ne zaman kullanılır | Derleme zamanı mı, çalışma zamanı mı? |
|---|---|---|
| `static_cast` | İlgili (compatible) tipler arasında: `int↔float`, `Base*→Derived*` (kontrolsüz), enum↔int... | Derleme zamanı, çalışma zamanında **hiç kontrol yok**. |
| `reinterpret_cast` | Alakasız tipler arasında ham bit kalıbını olduğu gibi yeniden yorumlamak: pointer↔integer, `TypeA*→TypeB*`. | Derleme zamanı, hiçbir dönüşüm/kontrol **yapmaz**, sadece derleyiciye "bu bitleri öbür tip gibi say" der. |
| `dynamic_cast` | Polymorphic (en az bir `virtual` fonksiyonu olan) bir hiyerarşide, çalışma zamanında **gerçek tipi güvenli şekilde** aşağı/yana cast etmek. | **Çalışma zamanında** RTTI (Run-Time Type Information) ile kontrol eder; başarısız olursa pointer için `NULL`, referans için `std::bad_cast` fırlatır. |
| `const_cast` | Sadece `const`/`volatile` niteleyicisini eklemek/kaldırmak. | Derleme zamanı; bu modülde hiç kullanılmıyor çünkü hiçbir egzersik bunu gerektirmiyor. |

Üç egzersik üç farklı satırı test ediyor: ex00 `static_cast` (güvenli, ilgili
sayısal tipler arası), ex01 `reinterpret_cast` (pointer↔integer, alakasız
tipler arası ham dönüşüm), ex02 `dynamic_cast` (polymorphic hiyerarşide
çalışma zamanı tip tespiti).

---

## 1. ex00 — `ScalarConverter`

**Amaç:** Bir string'in **hangi C++ scalar literal tipini** temsil
ettiğini kendi kodunuzla tespit etmek, sonra onu **açıkça** (`static_cast`
ile) diğer üç scalar tipe dönüştürmek — hepsini overflow/NaN/Inf/gösterilemez
karakter gibi kenar durumlarını gözeterek.

### Neden `static_cast`?

`char`, `int`, `float`, `double` hepsi **ilgili sayısal tiplerdir** — aralarında
derleyicinin bildiği, tanımlı, örtük (implicit) bile olabilecek dönüşüm
kuralları var (`int` bir `float`'a "makul" şekilde çevrilebilir). Bu tam
olarak `static_cast`'in var olma sebebi: "bu iki tip birbiriyle ilişkili,
dönüşüm kuralı derleyici tarafından biliniyor, bana bunu **açıkça** (implicit
conversion'a güvenmeden, niyetimi belli ederek) yap" demek.
`reinterpret_cast` burada **yanlış** olurdu çünkü o ham bit kalıbını
olduğu gibi yeniden yorumlar (örn. bir `double`'ın bitlerini `int` gibi okur)
— sayısal *değeri* korumaz, sadece bellek temsilini değiştirir; bizim
istediğimiz tam tersi: **değeri** koruyarak tipi değiştirmek.

### `ScalarConverter` neden hiç nesnelenemez?

```cpp
class ScalarConverter
{
public:
    static void convert(const std::string &literal);

private:
    ScalarConverter();
    ScalarConverter(const ScalarConverter &other);
    ScalarConverter &operator=(const ScalarConverter &other);
    ~ScalarConverter();
};
```

Sınıfın **hiç üye değişkeni yok** — sadece tek bir `static` fonksiyon
sunuyor, yani aslında bir "namespace" gibi davranıyor (C++98'de fonksiyonları
gruplamanın OOP yoluyla eş değeri). Subject açıkça "kullanıcı tarafından
nesnelenemesin" dediği için dört özel üye fonksiyon (default/copy
constructor, copy assignment, destructor) `private` yapıldı. Bunlar hiçbir
zaman çağrılmayacağı için (`convert` bir `static` fonksiyon, `this`
gerektirmiyor) `.cpp`'de gövdeleri boş bırakıldı — OCF'in "dörtlüsü" burada
**biçimsel olarak var ama işlevsiz**, çünkü sınıfın zaten hiç durumu
(state) yok.

### Tip tespiti — `classify()`

```cpp
enum LiteralType { T_CHAR, T_INT, T_FLOAT, T_DOUBLE, T_INVALID };

static LiteralType classify(const std::string &literal, double &value);
```

Sırayla kontrol ediliyor (ilk eşleşen kazanır):

1. **Char**: `literal.size() == 3 && literal[0] == '\'' && literal[2] == '\''`
   — yani tam olarak `'x'` kalıbı. `value`, o karakterin ASCII kodu olarak
   set edilir.
2. **Double pseudo-literal**: tam olarak `"nan"`, `"+inf"`, `"-inf"` ise,
   `value` sırasıyla `std::numeric_limits<double>::quiet_NaN()` /
   `infinity()` / `-infinity()` olarak set edilir (`<limits>`'ten).
3. **Float pseudo-literal**: `"nanf"`, `"+inff"`, `"-inff"` — aynı mantık,
   sadece `f` son eki farklı bir "kaynak tip" olduğunu işaretliyor
   (dönüşüm mantığı aynı, çünkü NaN/Inf hem float hem double'da var).
4. **Float**: son karakter `'f'` ise, `f` çıkarılıp geri kalan kısmın
   **tam olarak** geçerli bir sayı olup olmadığı `strtod` + `endptr`
   kontrolüyle doğrulanır (`isValidNumber`).
5. **Int**: `strtol` + `endptr` kontrolüyle **tüm string**in geçerli bir
   tamsayı olduğu doğrulanır (`isValidInt`) — `strtol` bir `'.'`
   gördüğünde durur, dolayısıyla `endptr` string sonuna ulaşmaz ve `4.2`
   gibi bir girdi otomatik olarak bu dalı **geçemez** (ekstra bir `.`
   kontrolüne gerek yok).
6. **Double**: yukarıdakilerin hiçbiri değilse ve `isValidNumber` geçiyorsa.
7. Hiçbiri değilse **`T_INVALID`** — dört satırın hepsi `impossible`
   basılır.

`isValidNumber`/`isValidInt` helper'ları, subject'in izin verdiği "string'i
sayıya çevirme yardımcı fonksiyonu" (`strtod`/`strtol`) kullanıyor, ama
**"tüm işi yapmıyor"**: asıl tip tespiti (hangisi char, hangisi
pseudo-literal, hangisi int/float/double) ve `endptr`'in string sonuna
ulaşıp ulaşmadığının kontrolü (yani "42abc" gibi kısmen sayısal
string'lerin reddedilmesi) tamamen elle yazıldı.

### Neden tek bir `double value` üzerinden çalışılıyor?

`classify()` literal'in tipini ne olursa olsun (char kodu, pseudo NaN/Inf,
gerçek sayı) **tek bir `double`ya** indirger. Bunun nedeni: dört hedef
tipin (`char`, `int`, `float`, `double`) hepsi bu tek değerden **aynı
mantıkla** türetilebilir — kaynağın orijinal biçiminin (`'a'` mı `97` mı
`97.0` mı olduğu) sonraki adımda hiçbir önemi kalmıyor. Bu, dört ayrı
"eğer kaynak char'sa şöyle, int'se böyle..." dalı yazmak yerine kodu
büyük ölçüde sadeleştiriyor.

### Aralık/geçerlilik kontrolleri

```cpp
static void printChar(double value)
{
    if (std::isnan(value) || std::isinf(value) ||
        value < static_cast<double>(std::numeric_limits<char>::min()) ||
        value > static_cast<double>(std::numeric_limits<char>::max()))
    { std::cout << "char: impossible\n"; return; }
    char c = static_cast<char>(value);
    if (std::isprint(static_cast<unsigned char>(c)))
        std::cout << "char: '" << c << "'\n";
    else
        std::cout << "char: Non displayable\n";
}
```

- `std::isnan` / `std::isinf` (`<cmath>`): NaN veya sonsuzluk bir `char`'a
  ya da `int`'e **anlamlı şekilde sığmaz** — bu yüzden `impossible`.
- `std::numeric_limits<char>::min()/max()` (`<limits>`): `char`'ın işaretli
  mi işaretsiz mi olduğu platforma bağlıdır (implementation-defined); sabit
  `-128`/`127` yazmak yerine `numeric_limits` kullanmak bunu **taşınabilir**
  yapıyor.
- Aralık içinde ama **gösterilemez** (örn. `0` → NUL karakteri) ise
  `std::isprint` (`<cctype>`) bunu yakalıyor — subject'in "Non displayable"
  mesajı tam olarak bu durum için.
- `int` için aynı desen `std::numeric_limits<int>::min()/max()` ile
  tekrarlanıyor; `99999999999999` gibi bir literal int **formatında**
  olsa da (sadece rakamlardan oluşuyor) değeri `INT_MAX`'i aştığı için
  `int: impossible` basılır — ama aynı değer `double`/`float` için
  sorunsuzdur (sadece hassasiyet kaybı olur, bu bir hata değildir).

### `float`/`double` yazdırma — neden `std::fixed << std::setprecision(1)`?

```cpp
static std::string formatFixed(double v)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << v;
    return oss.str();
}
```

`std::cout`'un varsayılan formatlaması `42.0`'ı `"42"` olarak basar (ondalık
kısmı `0` ise siler) — ama subject'in istediği çıktı her zaman en az bir
ondalık basamak gösteriyor (`0.0f`, `42.0f`, `4.2f`). `std::fixed`, `%g`
yerine `%f` tarzı sabit-ondalık formata zorluyor; `std::setprecision(1)`
her zaman tam olarak 1 ondalık basamak garantiliyor. Bu subject'teki tüm
örnek çıktılarla birebir eşleşiyor; daha fazla basamak (örn.
`max_digits10`) kasıtlı olarak tercih edilmedi çünkü hem örneklerle
uyuşmuyor hem de gereksiz karmaşıklık katardı.

`NaN`/`Inf` özel durumları `formatFixed`'den önce ayrıca ele alınıyor:
`float: nanf`, `double: nan`, `float: +inff`/`-inff`,
`double: +inf`/`-inf` (pozitif sonsuzluk için `+` işareti, girdi
pseudo-literal'lerinin kendi isimlendirmesiyle — `+inf`/`+inff` —
simetrik olsun diye tercih edildi).

### `main.cpp`

`argv[1]` verilirse tek bir literal `convert` edilir (subject'in
`./convert X` örneğine birebir uyar); argüman verilmezse önceden
tanımlı bir test dizisi (`0`, `42`, `-42`, taşan bir tamsayı, float/double
literaller, char literaller, tüm pseudo-literal varyasyonları) sırayla
denenir. Böylece hem tekil hem toplu test aynı programla yapılabiliyor.

---

## 2. ex01 — `Serializer`

**Amaç:** Bir pointer'ı, değerini kaybetmeden **tamsayı olarak** taşımayı
(ve geri dönüştürmeyi) öğretmek — genelde network/dosya serileştirme,
opak handle'lar (`void*` yerine `uintptr_t` taşımak) gibi düşük seviye
senaryolarda karşılaşılan bir teknik.

### Neden `reinterpret_cast`?

Bir pointer ile bir tamsayı **ilgisiz tiplerdir** — aralarında hiçbir
sayısal ya da mantıksal dönüşüm kuralı yoktur (`static_cast` burada
**derlenmez bile**, bir pointer'ı doğrudan bir tamsayıya `static_cast`
ile çevirmeye izin verilmez). İstenen şey değeri "anlamak" değil,
pointer'ın bellek adresini oluşturan **ham bit kalıbını** olduğu gibi bir
tamsayı değişkeninde saklamak, sonra aynı bitleri geri pointer olarak
okumak. Bu tam olarak `reinterpret_cast`'in tanımı: **hiçbir dönüşüm
mantığı çalıştırmadan**, aynı bitleri farklı bir tipin temsilcisi olarak
yeniden yorumlamak.

### `Data.hpp` — neden ayrı bir dosyada?

```cpp
struct Data
{
    int         id;
    std::string name;
    double      score;
};
```

Subject "boş olmayan bir `Data` struct'ı, kendi dosyalarında" diye açıkça
istiyor. `struct` (class değil) çünkü sadece veri taşıyor, davranışı yok —
C++'ta `struct`/`class` arasındaki tek fark varsayılan erişim
belirleyicisidir (`struct` → `public`, `class` → `private`); burada tüm
alanların dışarıdan erişilebilir olması gerektiği için `struct` daha
doğal bir seçim.

### `Serializer` — `ScalarConverter` ile aynı desen

```cpp
class Serializer
{
public:
    static uintptr_t serialize(Data *ptr);
    static Data *deserialize(uintptr_t raw);
private:
    Serializer();
    Serializer(const Serializer &other);
    Serializer &operator=(const Serializer &other);
    ~Serializer();
};
```

Yine hiç durumu olmayan, sadece iki `static` fonksiyon sunan, kullanıcı
tarafından nesnelenemeyen bir sınıf — ex00'daki `ScalarConverter` ile
birebir aynı gerekçe.

```cpp
uintptr_t Serializer::serialize(Data *ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}

Data *Serializer::deserialize(uintptr_t raw)
{
    return reinterpret_cast<Data *>(raw);
}
```

`uintptr_t` (`<stdint.h>`), standart kütüphanenin **"bir pointer'ı
kayıpsız tutabilecek büyüklükte işaretsiz tamsayı"** garantisi verdiği
özel bir tiptir — platforma göre boyutu değişir (64-bit sistemde 64 bit),
bu yüzzden `unsigned long` gibi "muhtemelen yeterli" bir tip yerine
kasıtlı olarak `uintptr_t` kullanılıyor.

### `main.cpp` — round-trip testi

Bir `Data` nesnesi oluşturulup adresi `serialize()`'a veriliyor, dönen
`uintptr_t` `deserialize()`'a veriliyor, sonuç orijinal adresle `==` ile
karşılaştırılıyor. Testte ayrıca `back->id`/`back->name`/`back->score`
basılarak, geri dönen pointer'ın gerçekten **aynı nesneye** eriştiği (bit
kalıbı bozulmadan taşındığı) doğrulanıyor.

---

## 3. ex02 — Gerçek tipi tespit etmek (`Base`, `A`, `B`, `C`)

**Amaç:** Polymorphic bir hiyerarşide, elimizde sadece bir `Base*`/`Base&`
varken, **çalışma zamanında** o nesnenin gerçekte hangi alt sınıftan
olduğunu `typeid` kullanmadan (RTTI'nin daha "manuel" ama tip-güvenli yolu
olan `dynamic_cast` ile) tespit etmek.

### Neden `dynamic_cast`?

`static_cast` ile bir `Base*`'i `A*`'a çevirmek **her zaman "başarılı"
görünür** — derleyici seni durdurmaz, ama eğer nesne gerçekte bir `A`
değilse (mesela bir `B`'yse), elde ettiğiniz pointer geçersizdir ve onu
kullanmak **undefined behavior**'dır; hiçbir çalışma zamanı kontrolü
yoktur. `dynamic_cast` ise **RTTI** kullanarak nesnenin gerçek (dinamik)
tipini çalışma zamanında kontrol eder: cast geçerliyse doğru pointer'ı
döner, geçersizse pointer formunda `NULL`, referans formunda
`std::bad_cast` fırlatır. Bu egzersizin **tam konusu** "gerçek tipi
güvenle tespit etmek" olduğu için `dynamic_cast` biricik doğru seçim —
tam da bunun için var.

### `Base`, `A`, `B`, `C` neden OCF'ten muaf?

```cpp
class Base
{
public:
    virtual ~Base();
};

class A : public Base {};
class B : public Base {};
class C : public Base {};
```

Subject bu dört sınıf için **açıkça** OCF şartını kaldırıyor (CPP05'teki
exception sınıflarına benzer bir istisna): bunlar hiçbir veri taşımıyor,
tek amaçları "farklı, tespit edilebilir tipler" olmak. `A`/`B`/`C` bomboş
— derleyicinin **örtük olarak ürettiği** default constructor/destructor
yeterli, elle hiçbir şey yazmaya gerek yok (bu yüzden `.cpp` dosyaları da
yok — implicit üyeler için tanımlanacak bir gövde zaten olmuyor).

`Base` tek bir şey tanımlıyor: **`public virtual` destructor**. Neden
zorunlu?

```cpp
Base *p = generate();   // gerçek tip A, B ya da C olabilir
...
delete p;                // Base* üzerinden silme
```

`~Base()` `virtual` olmasaydı, `delete p` **sadece** `Base::~Base()`'i
çağırırdı — nesnenin gerçek tipi `A`/`B`/`C` olsa bile o alt sınıfın
(varsa) kendi temizlik mantığı hiç çalışmaz, bu klasik bir **memory
leak/UB kaynağı** olurdu (CPP05 ex03'teki `AForm` ile birebir aynı
gerekçe). `virtual` sayesinde `delete p` her zaman **gerçek dinamik
tipin** destructor zincirini doğru çalıştırır.

Destructor gövdesi neden `.cpp`'de? Modülün genel kuralı: şablon
(template) olmayan hiçbir fonksiyon gövdesi header'a yazılmaz — trivial
bir `{}` bile olsa `Base.cpp`'ye konuyor.

### `generate()` — rastgele bir alt sınıf üretmek

```cpp
Base *generate(void)
{
    switch (std::rand() % 3)
    {
        case 0: return new A();
        case 1: return new B();
        default: return new C();
    }
}
```

`std::rand() % 3` ile 0/1/2 arası bir sayı üretilip `A`/`B`/`C`'den biri
`new` ile üretiliyor, dönüş tipi kasıtlı olarak `Base*` — çağıran taraf
(`main.cpp`) hangi somut tiple karşı karşıya olduğunu **bilmiyor**, tam
olarak `identify()`'ın çözmesi gereken problem bu. Tohum (`std::srand`)
`main.cpp`'de `std::time(NULL)` ile bir kere set ediliyor, her çalıştırmada
farklı bir dağılım elde edilsin diye.

### `identify(Base *p)` — pointer formu

```cpp
void identify(Base *p)
{
    if (dynamic_cast<A *>(p))
        std::cout << "A\n";
    else if (dynamic_cast<B *>(p))
        std::cout << "B\n";
    else if (dynamic_cast<C *>(p))
        std::cout << "C\n";
}
```

Pointer formunda `dynamic_cast` başarısız olduğunda **exception
fırlatmaz**, sadece `NULL` döner — bu yüzden sonucu doğrudan bir `if`
koşulunda kontrol etmek yeterli, `try/catch` gerekmiyor. Üç `dynamic_cast`
sırayla deneniyor, ilk başarılı olan gerçek tipi verir (bir nesne aynı
anda hem `A` hem `B` olamayacağı için sıralamanın bir önemi yok).

### `identify(Base &p)` — referans formu, neden pointer içeremiyor?

Subject bu fonksiyonun içinde **pointer kullanmayı yasaklıyor** — yani
`&p` alıp pointer overload'una devretmek kabul edilmez, gerçekten
**referans formunda** `dynamic_cast` kullanılmalı:

```cpp
void identify(Base &p)
{
    try
    {
        (void)dynamic_cast<A &>(p);
        std::cout << "A\n";
        return;
    }
    catch (...) { }
    try { ... dynamic_cast<B &>(p) ... }
    catch (...) { }
    try { ... dynamic_cast<C &>(p) ... }
    catch (...) { std::cout << "unknown\n"; }
}
```

Referans formunda `dynamic_cast` başarısız olduğunda `NULL` **döndüremez**
(bir referans hiçbir zaman "null" olamaz, dilin garantisi budur) — bunun
yerine `std::bad_cast` **fırlatır**. Bu yüzden her deneme kendi
`try/catch`'i içinde yapılıyor: cast başarılıysa tipi basıp `return`;
başarısızsa exception yakalanıp bir sonraki tipe geçiliyor.

**Neden `catch (std::bad_cast &)` değil de `catch (...)`?**
`std::bad_cast` standart olarak `<typeinfo>` başlığında tanımlıdır, ama
bu egzersizde `<typeinfo>` include etmek **açıkça yasak** (bu da dolaylı
olarak `typeid`'i de yasaklıyor). `std::bad_cast`'i isimlendirmeden (yani
`<typeinfo>`'ya hiç ihtiyaç duymadan) yakalamanın standart yolu,
**hangi tip fırlatıldığını umursamayan** `catch (...)` bloğudur — bu hem
kuralı ihlal etmiyor hem de burada mükemmel yeterli, çünkü zaten tek
olası exception `std::bad_cast`.

### `main.cpp`

`generate()` 8 kez çağrılıyor; her seferinde hem `identify(Base*)` hem
`identify(Base&)` (`*p` ile referansa çevrilip) çağrılıyor — ikisinin de
**aynı sonucu** verdiği gözlemlenebiliyor, çünkü ikisi de aynı gerçek
nesneye bakıyor. Her döngüde üretilen nesne `delete p;` ile temizleniyor
— `virtual ~Base()` sayesinde leak yok.

---

## 4. Bu modülde öğrenilen C++ kavramlarının özeti

- **`static_cast`**: İlgili/uyumlu tipler (sayısal tipler, bilinen
  hiyerarşi yönleri) arasında, derleme zamanı kontrollü, **niyet
  belirten** açık dönüşüm.
- **`reinterpret_cast`**: Alakasız tipler (pointer↔integer, tip↔tip)
  arasında hiçbir dönüşüm mantığı çalıştırmadan, ham bit kalıbını olduğu
  gibi yeniden yorumlamak.
- **`dynamic_cast`**: Polymorphic hiyerarşilerde RTTI ile **çalışma
  zamanında** güvenli tip kontrolü/downcast; pointer formunda `NULL`,
  referans formunda `std::bad_cast` ile başarısızlık bildirir.
- **Cast seçiminin "neden"i**: Her cast farklı bir garanti/riski temsil
  ettiği için hangisinin kullanılacağı **rastgele değil**, dönüşümün
  doğasına (ilgili tipler mi, ham bit yeniden yorumu mu, polymorphic tip
  kontrolü mü) göre seçilir.
- **`strtod`/`strtol` + `endptr` ile format doğrulama**: Bir string'in
  "tamamen" geçerli bir sayı olup olmadığını (kısmi eşleşmeleri
  reddederek) kontrol etmenin C++98 yolu.
- **`std::numeric_limits<T>`**: Bir tipin min/max değerlerine, platforma
  bağlı sabit sayılar yazmadan, taşınabilir şekilde erişmek.
- **`std::isnan`/`std::isinf`/`std::isprint`**: Özel kayan nokta
  değerlerini (NaN, sonsuzluk) ve gösterilemez karakterleri tespit etmek.
- **Nesnelenemez yardımcı sınıflar (`private` OCF üyeleri)**: Sadece
  `static` fonksiyon sunan, hiç durumu olmayan bir sınıfın kullanıcı
  tarafından `new`/kopyalanamayacak şekilde kilitlenmesi.
- **`uintptr_t`**: Bir pointer'ı kayıpsız taşıyabilecek büyüklükte,
  platforma göre boyutu değişen işaretsiz tamsayı tipi.
- **`virtual` destructor + `delete` (base class pointer üzerinden)**:
  Polymorphic bir hiyerarşide doğru destructor zincirinin çalışmasını
  garanti etmek — aksi halde alt sınıfın kaynakları hiç temizlenmez.
- **OCF istisnası (CPP05'teki exception sınıflarıyla aynı desen)**: Hiç
  veri taşımayan, sadece "tespit edilebilir tip" olarak var olan
  sınıfların (`A`, `B`, `C`) default/copy/assign/destructor'ı elle
  yazmaya gerek olmadığı, derleyicinin örtük ürettiği üyelerin yeterli
  olduğu.
- **`catch (...)`**: Fırlatılan exception'ın tipini isimlendirmeden
  (dolayısıyla o tipin başlığını include etmeye ihtiyaç duymadan)
  herhangi bir exception'ı yakalamak.
