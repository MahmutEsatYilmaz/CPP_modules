# CPP08 — Templated containers, iterators, algorithms

Bu modülün amacı, C++'ın **Standard Template Library (STL)**'sini —
container'lar (`std::vector`, `std::list`, `std::stack`...), iterator'lar ve
`<algorithm>` fonksiyonları (`std::find`, `std::sort`, `std::min_element`,
`std::max_element`) — kendi kodunuzu yazmak yerine **doğrudan kullanmayı**
öğretmek. Aşağıda önce bu modülün önceki modüllerle nasıl çeliştiği, sonra
her egzersizin ne yaptığı ve neden öyle tasarlandığı anlatılıyor.

---

## 0. Kural tersine döndü: CPP06/CPP07'de yasak, CPP08'de zorunlu

Bu, modülün en kritik ve en kolay kaçırılan noktası, o yüzden en başta ayrı
bir bölüm olarak vurgulanıyor.

**CPP06 ve CPP07'de** subject açıkça STL container'larını ve `<algorithm>`'i
**yasaklıyordu**. O modüllerin amacı, C++'ın temel yapı taşlarını (pointer,
reference, template, dinamik `new[]`/`delete[]` dizileri, kendi başınıza
yazdığınız arama/dönüştürme döngüleri) elle inşa edip nasıl çalıştıklarını
kavramaktı — STL'nin "arkasında" ne olduğunu anlamadan STL'yi kullanmak
öğretici olmazdı.

**CPP08'de bu kural tam tersine döndü.** Subject artık açıkça şunu söylüyor:
bu egzersizler STL kullanılmadan da çözülebilir, ama STL'siz bir çözüm
**"very bad grade" (çok kötü not)** ile sonuçlanır — çünkü modülün asıl amacı
STL'yi *kullanmayı* öğretmek, STL'yi *yeniden icat etmeyi* değil. Bunun
nedeni basit: CPP06/07'de container'ların/algoritmaların içini elle inşa
ederek "bunlar nasıl çalışıyor" öğrenildi; CPP08'de artık "bu tekerleği
tekrar icat etme, standart kütüphane bunun için var, doğru yerde doğru aracı
seç" öğreniliyor. Gerçek hayatta hiçbir profesyonel C++ geliştiricisi kendi
`find`'ını ya da kendi `sort`'unu yazmaz — STL, onlarca yıllık optimize
edilmiş, test edilmiş, hatasız kod sunuyor; onu görmezden gelip elle bir
arama döngüsü yazmak hem zaman kaybı hem hataya açık.

Bu yüzden bu projede her fırsatta STL'ye başvuruldu:
`std::find` (ex00), `std::vector`, `std::sort`, `std::min_element`,
`std::max_element` (ex01), `std::stack`'ten inheritance ve onun `iterator`
tipini yeniden kullanmak (ex02). Elle yazılmış hiçbir arama/sıralama/min-max
döngüsü yok — hepsi `<algorithm>`'den geliyor.

---

## 1. ex00 — `easyfind`

**Amaç:** Fonksiyon template'leri ile generic (tipten bağımsız) kod yazmayı
ve `std::find`'ı kullanmayı öğretmek.

```cpp
template <typename T>
typename T::iterator easyfind(T &container, int value)
{
	typename T::iterator it = std::find(container.begin(), container.end(), value);
	if (it == container.end())
		throw NotFoundException();
	return (it);
}
```

- **`template <typename T>`**: `T` burada "bir integer container" anlamına
  geliyor — ama fonksiyon bunun `std::vector<int>` mi, `std::list<int>` mi,
  `std::deque<int>` mi olduğunu **hiç bilmiyor ve bilmesine gerek yok**.
  Tek varsayım: `T`'nin `begin()`/`end()` metotları ve bir `T::iterator` tipi
  olması — yani **forward iterator** sağlayan herhangi bir sequence
  container.
- **`typename T::iterator`**: `T` bir template parametresi olduğu için,
  derleyici `T::iterator`'ın bir *tip* mi yoksa bir *değer* mi olduğunu
  önceden bilemez (bu **dependent name** denen bir durum). `typename`
  anahtar kelimesi derleyiciye "bu bir tip, değişken değil" der. Bu olmadan
  derleme hatası alınır.
- **`std::find(first, last, value)`**: `<algorithm>`'den gelen, `[first,
  last)` aralığında `value`'ya eşit ilk elemanı arayan generic algoritma.
  İçeride basitçe `first`'ten `last`'e kadar `*it == value` kontrolü yapan
  bir döngüdür — ama bunu elle yazmak yerine STL'nin sağladığı, her iterator
  tipiyle çalışan, test edilmiş halini kullanmak tam olarak bu modülün
  istediği şey. Bulamazsa `last` (yani `container.end()`) döner; bu da
  "bulunamadı" durumunun kontrolünü (`it == container.end()`) basitleştirir.
- Neden `std::vector`, `std::list`, `std::deque` **aynı fonksiyonla**
  çalışıyor? Çünkü hepsi aynı **iterator arayüzünü** (increment `++it`,
  dereference `*it`, eşitlik `it == it`) sağlıyor — bellekte nasıl
  saklandıkları (vector: bitişik dizi, list: çift bağlı liste, deque:
  segmentli dizi) `easyfind`'ı hiç ilgilendirmiyor. Bu, **iterator
  soyutlamasının** (abstraction) tam gücü: algoritma, veri yapısının iç
  detayından tamamen bağımsız yazılabiliyor.
- **`NotFoundException`**: `std::exception`'dan türeyen, `what()`'i
  override eden küçük bir sınıf — CPP05'ten beri alışılan desen. Bulunamama
  durumu bir hata değeri (`-1` gibi) yerine exception ile ifade edildi,
  çünkü dönüş tipi zaten `iterator` — "geçersiz bir iterator" döndürmek
  (örn. `end()`) çağıranın bunu kontrol etmeyi *unutabileceği* bir tuzak
  yaratır; `throw` ile bu unutulamaz hale getiriliyor.

`main.cpp`, her üç container tipiyle hem "bulunan" hem "bulunamayan" bir
değer test ediyor — toplam 6 senaryo, hepsi tek bir template fonksiyon
(`testFound`) üzerinden çalıştırılıyor.

---

## 2. ex01 — `Span`

**Amaç:** `std::vector`'ı bir sınıfın iç veri yapısı olarak kullanmayı,
üye fonksiyon template'lerini (`addNumbers`) ve `<algorithm>`'in
min/max/sort fonksiyonlarını öğretmek.

### Neden default constructor yok?

```cpp
private:
	Span();   // private + tanımsız: kullanılmasın diye
public:
	explicit Span(unsigned int n);
```

OCF (Orthodox Canonical Form) normalde 4 üyeyi zorunlu kılar: default
constructor, copy constructor, copy assignment, destructor. Ama subject
burada açıkça şunu diyor: *"N is the only parameter passed to the
constructor"* — yani `Span`'in **anlamlı bir varsayılan hali yok**. Bir
`Span`'in kapasitesi olmadan var olması mantıksız (kapasitesi 0 olan bir
Span'e hiç sayı eklenemez, bu da işe yaramaz bir nesne olurdu). Bu yüzden:

- Default constructor **kasıtlı olarak yazılmadı** (ya da `private` yapılıp
  tanımsız bırakılabilirdi — burada tercih tamamen atlamak oldu, çünkü
  `private` bir bildirim varlığını ima eder, hâlbuki burada "bu constructor
  hiç yok" mesajı daha net).
- Copy constructor, copy assignment operator ve destructor **hâlâ var** ve
  anlamlı — bir `Span`'i kopyalamak (`Span s2 = s1;`) gayet mantıklı bir
  işlem, kapasitesi ve içindeki sayılarıyla birlikte kopyalanabilir. Subject
  bunları istisna tutmuyor, sadece default constructor'ı gereksiz kılıyor.
  Bu, OCF kuralının "class'ın anlamına uymayan kısmı atlanabilir" istisnası
  — CPP05'te exception sınıflarının default constructor'ı zorunlu
  tutulmaması gibi bir durum.
- `explicit` neden var? `Span sp = 5;` gibi **örtük (implicit) dönüşümü**
  engellemek için — `unsigned int`'ten `Span`'e sessiz sedasız dönüşüm
  yapılmasın, `Span sp(5)` ya da `Span sp = Span(5)` gibi **açık** bir
  çağrı zorunlu olsun.

### `std::vector<int>` neden backing store?

```cpp
private:
	unsigned int _maxSize;
	std::vector<int> _numbers;
```

`vector`, bitişik bellekte tutulan, `push_back` ile büyüyen, rastgele
erişimli (`operator[]`) bir container — tam olarak "en fazla N tane int
tutacağım, sona ekleyeceğim, sonra sıralayıp min/max arayacağım"
senaryosuna uyan en doğal seçim. `reserve(n)` constructor'da çağrılarak
kapasitenin baştan ayrılması sağlanıyor (performans için, N elemana kadar
yeniden tahsis olmasın diye) — ama bu N'i **aşmayı engellemiyor**, sadece
bellek verimliliği; asıl kapasite kontrolü `addNumber`/`addNumbers`
içindeki manuel `if` kontrolüyle yapılıyor.

### `addNumber` vs `addNumbers` (iterator-range template)

```cpp
void addNumber(int n);   // tek sayı

template <typename InputIterator>
void addNumbers(InputIterator first, InputIterator last)
{
	// count = [first, last) aralığındaki eleman sayısı
	if (this->_numbers.size() + count > this->_maxSize)
		throw Span::FullException();
	this->_numbers.insert(this->_numbers.end(), first, last);
}
```

Bu, STL container'larının kendi `insert(first, last)` overload'larının
**birebir aynı deseni** — `std::vector::insert`, `std::set::insert` gibi
birçok container, tek tek eleman eklemenin yanında bir **iterator aralığı**
alıp topluca ekleyen bir overload da sunar. Subject burada tam olarak bunu
taklit etmemizi istiyor: `Span::addNumbers`, herhangi bir `InputIterator`
(bir `std::vector<int>::iterator`, bir ham `int*`, bir `std::list<int>::iterator`...)
alabiliyor, çünkü template parametreleştirilmiş — `easyfind`'daki gibi,
hangi container'dan geldiği önemli değil, sadece iterator arayüzü
sağlaması yeterli. Kapasite kontrolü, `insert` çağrılmadan **önce** tüm
aralığın büyüklüğü hesaplanıp yapılıyor — böylece "aralığın yarısı eklenip
sonra hata fırlatılan" tutarsız bir durum (yarım işlem) hiç oluşmuyor;
ya hep ya hiç.

### `shortestSpan()` / `longestSpan()` — `<algorithm>` kullanımı

```cpp
int Span::longestSpan() const
{
	std::vector<int>::const_iterator minIt = std::min_element(_numbers.begin(), _numbers.end());
	std::vector<int>::const_iterator maxIt = std::max_element(_numbers.begin(), _numbers.end());
	return (*maxIt - *minIt);
}
```

`longestSpan`, tüm sayılar arasındaki en büyük farkı (`max - min`) istiyor
— bunun için elle bir döngüyle en büyüğü/en küçüğü aramak yerine
`<algorithm>`'in hazır `std::min_element`/`std::max_element`'i kullanıldı;
her ikisi de `[first, last)` aralığında tek geçişte (O(n)) ilgili elemanı
bulan iterator döndürüyor.

`shortestSpan`, "herhangi iki sayı arasındaki en kısa mesafe"yi istiyor.
Bunu doğrudan `min_element`/`max_element` ile çözmek mümkün değil (o ikisi
sadece uç değerleri buluyor, ardışık farkları değil) — bunun yerine önce
`std::sort` ile bir kopya sıralanıyor (orijinal `_numbers` sırası
bozulmasın diye kopya alınıyor), sonra sıralı dizide **ardışık elemanlar
arasındaki farkların en küçüğü** aranıyor. Sıralı bir dizide iki elemanın
en yakın olabileceği durum her zaman komşu elemanlar arasındadır — bu
yüzden N² karşılaştırma yerine sadece N-1 ardışık fark karşılaştırması
yeterli. `std::sort` burada yine STL'nin hazır, optimize (introsort,
ortalama O(n log n)) algoritması olarak kullanılıyor, elle bir sıralama
yazılmadı.

İkisi de eleman sayısı 2'den azsa `NotEnoughNumbersException` fırlatıyor
(bir "mesafe" tanımlı olması için en az 2 sayı gerekiyor).

### `main.cpp` testleri

Önce subject'in birebir örneği çalıştırılıyor (`Span sp(5)`, 6-3-17-9-11
eklenip `shortestSpan()`/`longestSpan()` basılıyor) — çıktı tam olarak
beklenen `2` / `14`. Sonra ek testler: kapasiteyi aşan `addNumber` (exception
bekleniyor), 2'den az elemanla `shortestSpan()` (exception bekleniyor),
`addNumbers` ile bir `std::vector<int>`'ten aralık ekleme, kapasiteyi aşan
bir `addNumbers` çağrısı (exception bekleniyor). Son olarak subject'in
"en az 10.000 sayı ile test et" talebi karşılanıyor: `std::rand()` ile
10.000 elemanlı bir `std::vector<int>` dolduruluyor, **tek bir
`addNumbers(first, last)` çağrısıyla** hepsi `Span`'e ekleniyor, ardından
`shortestSpan()`/`longestSpan()` çağrılıp süre/ölçek sorununun olmadığı
(anında sonuç döndüğü) doğrulanıyor.

---

## 3. ex02 — `MutantStack`

**Amaç:** Container **adapter** kavramını, `std::stack`'in neden iterator
sağlamadığını ve bir standart tipten **public inheritance** ile onu
genişletmeyi öğretmek.

### `std::stack` neden zaten iterable değil?

`std::stack`, gerçek bir container değil, bir **container adapter**'dır:
kendi belleğini tutmaz, arkada başka bir container'ı (varsayılan olarak
`std::deque<T>`) **sarmalar** ve ona sadece kısıtlı bir arayüz (`push`,
`pop`, `top`, `size`, `empty`) sunar. Bu kısıtlama **bilinçli bir tasarım
kararı**: "stack" soyut veri yapısının anlamı zaten "sadece tepeye
eklersin, sadece tepeden çıkarırsın" — eğer serbestçe dolaşmaya izin
verilseydi, bu artık bir stack olmaktan çıkıp sıradan bir sequence
container'a dönerdi. Bu yüzden STL, `stack`'e kasıtlı olarak `begin()`/
`end()` **vermemiştir**.

### Çözüm: inheritance ile genişletmek

```cpp
template <typename T>
class MutantStack : public std::stack<T>
{
public:
	typedef typename std::stack<T>::container_type::iterator iterator;
	...
	iterator begin() { return (this->c.begin()); }
	iterator end()   { return (this->c.end()); }
};
```

- **Public inheritance** (`: public std::stack<T>`) seçildi çünkü
  `MutantStack`, `std::stack`'in **tüm** public arayüzünü (`push`, `pop`,
  `top`, `size`, `empty`) olduğu gibi miras alıp kullanıcıya sunmak
  istiyor — private/protected inheritance bu arayüzü dışarıya kapatırdı.
  Ayrıca subject'in testi `std::stack<int> s(mstack);` satırıyla bir
  `MutantStack<int>`'ten doğrudan bir `std::stack<int>` inşa etmeyi
  bekliyor — bu ancak `MutantStack`'in bir `std::stack` **olması**
  (is-a ilişkisi, yani public inheritance) sayesinde mümkün, çünkü
  `std::stack`'in copy constructor'ı bir `std::stack<T> const&` alıyor ve
  bir `MutantStack<T>&`, bir `std::stack<T> const&`'e **implicit olarak
  upcast** edilebiliyor (temel sınıfına referans/pointer daima geçerlidir).
- **`this->c`**: `std::stack`'in içinde, sarmaladığı gerçek container'ı
  tutan üye `protected Sequence c;` olarak tanımlıdır (standart, bu ismi
  ve erişilebilirliğini garanti eder). `protected` olduğu için türeyen bir
  sınıf (`MutantStack`) buna doğrudan erişebilir — normal bir üye gibi.
  `this->` öneki template bağlamında zorunlu: `c`, taban sınıftan
  (template-dependent base) geldiği için derleyici onu ismiyle bulamaz,
  `this->c` ya da `std::stack<T>::c` ile açıkça belirtmek gerekir.
- **`container_type::iterator`**: `std::stack`'in ikinci template
  parametresi (varsayılan `std::deque<T>`) `container_type` olarak
  `typedef`'lenmiştir. `c`'nin gerçek tipi bu, dolayısıyla onun
  `iterator`'ı da bu container'ın kendi iterator tipi. `begin()`/`end()`,
  `c.begin()`/`c.end()`'i olduğu gibi dışarı açıyor — hiçbir ekstra
  mantık yok, sadece **var olan bir şeyi görünür kılmak**.
- OCF üyeleri (`MutantStack()`, copy constructor, `operator=`,
  destructor) sadece taban sınıfa (`std::stack<T>`) devrediyor
  (pass-through) — çünkü `std::stack`'in kendi kopyalama semantiği zaten
  doğru (arkasındaki container'ı kopyalıyor), tekrar yazmaya gerek yok.

### `begin()` neden alttan (bottom) başlıyor, tepeden (top) değil?

`std::stack::push(x)`, arkadaki container üzerinde `c.push_back(x)`
çağırır — yani ilk push edilen eleman `c`'nin **başında** (`c[0]`), son
push edilen ise **sonunda** (`c.back()`, aynı zamanda `stack::top()`)
kalır. `begin()`/`end()`, `c.begin()`/`c.end()`'i olduğu gibi
döndürdüğünden, iterasyon **her zaman ilk push edilenden son push
edilene** doğru gider — yani "en altta olan"dan "en üstte olan"a. Bu,
subject'in verdiği ipucuyla birebir örtüşüyor: aynı push sırasıyla
doldurulmuş bir `std::list<int>` üzerinde `push_back` + `begin()`→`end()`
ile dolaşmak, **aynı sırayı** üretir — çünkü ikisi de aynı ekleme
sırasını, aynı yönde (baştan sona) geziyor. `begin()`'i tersine çevirmek
(örn. `rbegin()` kullanmak) bu eşleşmeyi bozardı; bu yüzden hiçbir tersine
çevirme yapılmadı, `c`'nin kendi doğal iterator sırası korundu.

### `main.cpp` testi

Subject'in birebir verdiği test kodu çalıştırıldı: `push(5)`, `push(17)`,
`top()` (`17` basar), `pop()`, `size()` (`1` basar), ardından `3, 5, 737, 0`
push edilip `begin()`/`end()` ile dolaşılıyor (bir `++it; --it;` no-op
turuyla iterator'ın ileri-geri hareket edebildiği de doğrulanıyor), ekrana
sırasıyla `5, 3, 5, 737, 0` basılıyor — bu tam olarak "en alttakinden en
üsttekine" sırası. Son satır `std::stack<int> s(mstack);`, public
inheritance sayesinde derlenip çalışıyor, `(void)s;` ile kullanılmayan
değişken uyarısı susturuluyor.

---

## 4. Bu modülde öğrenilen kavramların özeti

- **STL container'ları** (`std::vector`, `std::list`, `std::deque`,
  `std::stack`): Her birinin kendi bellek/erişim özellikleri var, ama ortak
  bir iterator arayüzü paylaşıyorlar.
- **Iterator soyutlaması**: `begin()`/`end()` + `++`/`*`/`==` — bir
  algoritmanın, altındaki veri yapısının iç detayından bağımsız yazılmasını
  sağlıyor (`easyfind`, `Span::addNumbers` bunun örnekleri).
- **`<algorithm>` fonksiyonları**: `std::find` (arama), `std::sort`
  (sıralama), `std::min_element`/`std::max_element` (uç değer bulma) — hepsi
  generic, herhangi bir iterator çiftiyle çalışıyor, elle yazılmadı.
- **Fonksiyon template'leri**: `easyfind<T>`, tek bir kod yazıp `vector`,
  `list`, `deque` ile aynı anda çalışabiliyor.
- **Üye fonksiyon template'leri**: `Span::addNumbers<InputIterator>`, STL
  container'larının `insert(first, last)` deseninin birebir taklidi.
- **Container'a-özgü OCF esnekliği**: `Span`'de default constructor'ın
  bilinçli olarak atlanması — OCF, "her class'ta anlamlı olan 4 üye"
  demek, "her class'ta *mutlaka* 4 üye" demek değil.
- **Container adapter kavramı**: `std::stack` (ve `std::queue`,
  `std::priority_queue`), gerçek bir container değil, başka bir container'ı
  kısıtlı bir arayüzle sarmalayan bir "cephe" (facade).
- **Standart bir tipten public inheritance**: `MutantStack : public
  std::stack<T>` — taban sınıfın tüm arayüzünü miras alıp üstüne yeni
  davranış (`begin`/`end`) eklemek, ve bu sayede taban sınıf tipine
  (`std::stack<T> const&`) implicit dönüşümün de çalışması.
- **`protected` üyelere türeyen sınıftan erişim**: `this->c` ile,
  template-dependent bir taban sınıfın `protected` üyesine erişim.
