# CPP07 - Yapılan Değişiklikler

Modül baştan tamamen boştu (klasörde sadece `CPP7.pdf` vardı) — ex00,
ex01, ex02 sıfırdan yazıldı.

## ex00 - whatever (swap / min / max)

Klasör yoktu, sıfırdan oluşturuldu:

- `whatever.hpp`: `swap`, `min`, `max` function template'leri.
  `min`/`max`'in "eşitlik durumunda ikinci parametreyi döndür" kuralı
  tek bir karşılaştırmayla sağlandı: `min` → `(a < b) ? a : b`, `max`
  → `(a > b) ? a : b` (eşitlikte her iki karşılaştırma da `false`
  döner, dolayısıyla otomatik olarak ikinci parametre — `b` — döner).
- `main.cpp`: Subject'teki örnek kod birebir kullanıldı (`int a=2,b=3`
  ve `std::string c="chaine1", d="chaine2"` üzerinde swap/min/max).
- `Makefile`: Verilen şablona göre sıfırdan yazıldı (`NAME=whatever`).

## ex01 - iter

Klasör yoktu, sıfırdan oluşturuldu:

- `iter.hpp`: `template <typename T, typename F> void iter(T *array,
  size_t const len, F f)` — dizinin her elemanında `f` çağırıyor. Tek
  bir template hem const hem non-const dizileri destekliyor: `T`,
  çağrıya verilen dizinin gerçek tipinden (const dahil) otomatik
  çıkarsanıyor, const'luk ihlali (const elemanı değiştirmeye çalışan
  bir `f`) derleyici tarafından kendiliğinden reddediliyor — ayrı bir
  const/non-const overload'a gerek kalmıyor.
- `main.cpp`: Non-const `int` dizisi üzerinde yazdırma ve yerinde
  ikiye katlama (mutasyon), `const int` dizisi üzerinde sadece okuma,
  `std::string` dizisi üzerinde yazdırma test edildi.
- `Makefile`: Verilen şablona göre sıfırdan yazıldı (`NAME=iter`).

## ex02 - Array

Klasör yoktu, sıfırdan oluşturuldu:

- `Array.hpp` + `Array.tpp`: `template <typename T> class Array`.
  Default constructor, `Array(unsigned int n)` (elemanlar `new
  T[n]()` ile value-initialize edilip `0`/`0.0` ile başlıyor, çöp
  değer kalmıyor), copy constructor + `operator=` (ikisi de **deep
  copy** — elemanlar tek tek yeni bir bloğa kopyalanıyor, aksi halde
  ham pointer'ın shallow-copy'lenmesi paylaşılan bellek ve destructor'da
  double-free'ye yol açardı), destructor (`delete[] _data`),
  `operator[]` hem mutable hem `const` overload (sınır dışı erişimde
  `std::out_of_range` fırlatıyor), `size() const`. Sadece ihtiyaç
  kadar `new[]` (preventive/max-size allocation yok).
- `main.cpp`: Boş dizi + `size()`, 5 elemanlı dizide value-init
  kontrolü (`a[i]==0`), `operator[]` ile eleman değiştirme, copy
  constructor ve `operator=` sonrası kopya/orijinal bağımsızlığının
  kanıtlanması, hem mutable hem `const` referans üzerinden sınır dışı
  erişimin `try/catch` ile yakalanması, tüm dizilerin `size()`
  değerlerinin son kontrolü.
- `Makefile`: Verilen şablona göre sıfırdan yazıldı (`NAME=array`).

## Durum

ex00, ex01, ex02 → her biri `make re` ile (Makefile'lar `c++ -Wall
-Wextra -Werror -std=c++98` kullanıyor, `g++` doğrudan çağrılmadı)
uyarısız/hatasız derleniyor. ex00'ın çıktısı çalıştırılıp subject'teki
örnekle karşılaştırıldı ve birebir eşleşiyor:

```
a = 3, b = 2
min( a, b ) = 2
max( a, b ) = 3
c = chaine2, d = chaine1
min( c, d ) = chaine1
max( c, d ) = chaine2
```

ex01 ve ex02 çalıştırılıp davranışları (const/non-const iter, deep
copy, out-of-bounds exception, value-initialization) manuel olarak
doğrulandı. Üç egzersizde de `make fclean` ile build artifact'leri
temizlendi, klasörde sadece kaynak dosyalar + Makefile kaldı.
