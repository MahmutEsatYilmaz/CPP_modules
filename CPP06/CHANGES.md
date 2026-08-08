# CPP06 - Yapılan Değişiklikler

Subject (CPP6.pdf) ile karşılaştırılarak ex00'daki boş scaffolding
dolduruldu, ex01 ve ex02 tamamen sıfırdan yazıldı.

## ex00 - ScalarConverter

`Makefile`, `ScalarConverter.hpp`, `ScalarConverter.cpp` klasörde vardı
ama üçü de 0 byte'tı; `main.cpp` hiç yoktu. Hepsi sıfırdan yazıldı:

- `ScalarConverter.hpp`: `convert(const std::string &literal)` tek
  `static` üyesine sahip, `private` default/copy constructor + copy
  assignment + destructor ile kullanıcı tarafından nesnelenemeyen bir
  sınıf.
- `ScalarConverter.cpp`: `classify()` literal'in tipini (char /
  int / float / double pseudo-literal dahil / geçersiz) tespit edip
  hepsini tek bir `double value`'ya indirgeyen bir fonksiyon;
  `strtod`/`strtol` + `endptr` kontrolüyle "tüm string tüketildi mi"
  doğrulaması yapılıyor (bu, `strtod`/`strtol`'un "asıl işi yapmayan bir
  yardımcı" olarak kullanılmasına izin veren subject kuralına uyuyor —
  tip tespiti ve kenar durumu mantığı elle yazıldı). `printChar`/`printInt`
  `std::numeric_limits<T>` ile aralık kontrolü yapıyor, `std::isnan`/
  `std::isinf`/`std::isprint` ile NaN/Inf/gösterilemez karakter
  durumlarını ayırıyor. `float`/`double` çıktısı `std::fixed <<
  std::setprecision(1)` ile subject'in örnek çıktılarıyla (`0.0f`,
  `42.0f`, `4.2f`...) birebir eşleşecek şekilde formatlanıyor. Tüm
  dönüşümler `static_cast` ile yapılıyor (Additional Rule gereği bu
  egzersize uygun cast).
- `main.cpp`: sıfırdan yazıldı. `argv[1]` verilirse tek literal
  `convert` edilir; verilmezse int/float/double/char, taşan tamsayı,
  tüm pseudo-literal varyasyonları ve gösterilemez-karaktere-dönüşen
  bir değeri kapsayan sabit bir test dizisi döngüyle denenir.
- Test: `./convert 0`, `./convert nan`, `./convert 42.0f` subject'in
  transcript'indeki çıktılarla **birebir** eşleşti.

## ex01 - Serializer

Klasör tamamen boştu, sıfırdan oluşturuldu:

- `Data.hpp`: boş olmayan (`int id`, `std::string name`, `double score`)
  bir `struct Data`, subject'in ayrıca istediği gibi kendi dosyasında.
- `Serializer.hpp/cpp`: `ScalarConverter` ile aynı "nesnelenemez, sadece
  static fonksiyon sunan" desende bir sınıf. `serialize(Data*)` →
  `uintptr_t` ve `deserialize(uintptr_t)` → `Data*` fonksiyonları
  `reinterpret_cast` ile yazıldı (pointer↔integer arasında ham bit
  kalıbı yeniden yorumu — bu egzersize uygun cast, `static_cast` burada
  zaten derlenmez).
- `main.cpp`: bir `Data` nesnesi oluşturup adresini `serialize()`'a
  verip dönen değeri `deserialize()`'a geri veren, sonucu orijinal
  adresle `==` karşılaştıran ve `back->id/name/score`'u basarak round-trip'in
  bit kaybı olmadan çalıştığını doğrulayan bir test yazıldı.

## ex02 - Base / A / B / C / identify

Klasör tamamen boştu, sıfırdan oluşturuldu:

- `Base.hpp/cpp`: subject'in istediği gibi sadece `public virtual
  ~Base();` — hiçbir başka üye yok. Gövde `.cpp`'de (modülün "header'da
  fonksiyon gövdesi olmaz" kuralı gereği, trivial olsa bile).
- `A.hpp`, `B.hpp`, `C.hpp`: `Base`'den `public` türeyen boş sınıflar.
  Subject bu dört sınıfı OCF'ten açıkça muaf tuttuğu için hiçbir
  default/copy/assign/destructor elle yazılmadı — derleyicinin örtük
  ürettiği üyeler yeterli, bu yüzden `A.cpp`/`B.cpp`/`C.cpp` de yok
  (yazılacak hiçbir gövde olmadığı için).
- `identify.hpp/cpp`: `generate()` (`std::rand() % 3` ile `A`/`B`/`C`'den
  birini `new` ile üretip `Base*` olarak döner), `identify(Base*)`
  (üç `dynamic_cast<X*>` deneyip ilk `NULL` olmayanı basar) ve
  `identify(Base&)` (pointer kullanmadan, `dynamic_cast<X&>`'i
  `try/catch` içinde deneyip başarısızlıkta fırlatılan exception'ı
  yakalayarak bir sonrakine geçer). `<typeinfo>` include etmek yasak
  olduğu için `std::bad_cast`'i isimle yakalamak mümkün değildi (derleme
  hatası verdi: "`bad_cast` in namespace `std` does not name a type");
  bunun yerine tipi hiç isimlendirmeyen `catch (...)` kullanıldı — kural
  ihlali yok, tek olası exception zaten `std::bad_cast`.
- `main.cpp`: `generate()` 8 kez çağrılıp her nesnede hem
  `identify(Base*)` hem `identify(Base&)` çağrılıyor (ikisinin aynı
  sonucu verdiği gözlemlenebiliyor), sonra `delete` ediliyor.

## Durum

ex00, ex01, ex02 → `c++ -Wall -Wextra -Werror -std=c++98` ile temiz
derleniyor. `make re` her klasörde hatasız/uyarısız tamamlandı,
çalıştırılan test çıktıları beklentilerle eşleşti, `make fclean` sonrası
klasörlerde sadece kaynak dosyalar + Makefile kaldı (derleme
artefaktları temizlendi).
