# CPP08 - Yapılan Değişiklikler

Bu modül başlangıçta tamamen boştu (sadece `CPP8.pdf` vardı, hiç kod
yoktu) — ex00, ex01, ex02 sıfırdan yazıldı.

## ex00 - easyfind

Klasör baştan yoktu, sıfırdan oluşturuldu:

- `easyfind.hpp`: `template <typename T> typename T::iterator
  easyfind(T &container, int value)` — `std::find` kullanarak container
  içinde ilk eşleşen değeri arıyor, bulamazsa `NotFoundException`
  (`std::exception`'dan türeyen küçük bir sınıf) fırlatıyor.
- `main.cpp`: `std::vector<int>`, `std::list<int>`, `std::deque<int>`
  üzerinde hem var olan hem olmayan bir değer aranarak 6 senaryo test
  edildi.
- `Makefile`: subject'in verdiği şablona göre yazıldı (`SRCS = main.cpp`).

## ex01 - Span

Klasör baştan yoktu, sıfırdan oluşturuldu:

- `Span.hpp/cpp`: İçeride `std::vector<int>` tutan `Span` sınıfı.
  Constructor sadece `unsigned int n` (kapasite) alıyor; default
  constructor bilinçli olarak **yazılmadı** (subject'in "N is the only
  parameter" ifadesi ve mantıksal olarak kapasitesiz bir Span'in anlamsız
  olması nedeniyle) — copy constructor, `operator=`, destructor ise var.
- `addNumber(int)`: kapasite doluysa `Span::FullException` fırlatıyor.
- `addNumbers(InputIterator first, InputIterator last)`: template üye
  fonksiyon, STL'nin `insert(first, last)` desenini taklit ediyor; aralık
  eklenmeden önce toplam boyut kontrol ediliyor (kısmi ekleme olmasın diye).
- `shortestSpan()`: bir kopyayı `std::sort` ile sıralayıp ardışık
  farkların minimumunu buluyor; `longestSpan()`: `std::min_element` /
  `std::max_element` ile `max - min` hesaplıyor. İkisi de 2'den az eleman
  varsa `Span::NotEnoughNumbersException` fırlatıyor.
- `main.cpp`: subject'in birebir örneği (`Span(5)`, 6-3-17-9-11 eklenip
  `shortestSpan()`/`longestSpan()` basılması — çıktı `2` / `14`, tam
  beklenen); ek olarak kapasite aşımı, yetersiz eleman, `addNumbers` ile
  aralık ekleme ve `addNumbers` kapasite aşımı senaryoları; son olarak
  subject'in istediği **10.000 sayılık** test — `std::rand()` ile
  doldurulmuş bir `std::vector<int>`'in tamamı tek bir `addNumbers(first,
  last)` çağrısıyla ekleniyor, `shortestSpan()`/`longestSpan()` anında
  sonuç veriyor.
- `Makefile`: `SRCS = main.cpp Span.cpp` ile yazıldı.

## ex02 - MutantStack

Klasör baştan yoktu, sıfırdan oluşturuldu:

- `MutantStack.hpp`: `std::stack<T>`'den **public inheritance** ile
  türeyen template sınıf. `container_type::iterator` typedef'lenip,
  `begin()`/`end()` `std::stack`'in `protected c` üyesi (`this->c`)
  üzerinden dışarı açılıyor. OCF üyeleri taban sınıfa pass-through.
- `main.cpp`: subject'in birebir verdiği test kodu — `push`, `top`, `pop`,
  `size`, ardından `begin()`/`end()` ile dolaşma, son olarak
  `std::stack<int> s(mstack);` (public inheritance sayesinde derleniyor).
- `Makefile`: `SRCS = main.cpp` ile yazıldı.

## Durum

ex00, ex01, ex02 → `c++ -Wall -Wextra -Werror -std=c++98` ile temiz
derleniyor (`make re`, uyarı/hata yok). Çalışma zamanı doğrulamaları:

- ex01: subject örneği tam olarak `2` / `14` basıyor; 10.000 sayılık
  `addNumbers` testi anında sonuç veriyor (leak yok, kapasite kontrolleri
  doğru çalışıyor).
- ex02: `top()` → `17`, `pop()` sonrası `size()` → `1`, ardından
  `begin()`→`end()` dolaşımı `5, 3, 5, 737, 0` (alttan üste, push sırası)
  basıyor — beklenen sırayla birebir eşleşiyor; `std::stack<int>
  s(mstack);` sorunsuz derleniyor.

`make fclean` ile tüm derleme çıktıları temizlendi, `git status` CPP08
altında sadece kaynak dosyaları (Makefile/.hpp/.cpp/.md) gösteriyor,
başka artefakt yok.
