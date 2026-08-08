# CPP05 - Yapılan Değişiklikler

Subject (CPP5.pdf) ile karşılaştırılarak ex00, ex01, ex02 içindeki hatalar düzeltildi, eksik kısımlar tamamlandı ve ex03 sıfırdan yazıldı.

## ex00 - Bureaucrat

- `Bureaucrat.cpp`: Default constructor'daki initializer-list sırası (`name("none"), grade(150)`) header'daki declare sırasıyla (`grade`, `name`) uyuşmuyordu, `-Werror=reorder` ile derlemeyi patlatıyordu. Sıra `grade(150), name("none")` olarak düzeltildi.
- `Makefile`: `SRCS` listesine yanlışlıkla eklenmiş `Bureaucrat.hpp` girdisi kaldırıldı (bu girdi `make clean` çalıştığında `.hpp` dosyasının silinmesine yol açıyordu).
- `main.cpp`: Bir catch bloğu içindeki anlamsız iç içe try/catch sadeleştirildi.

## ex01 - Form

- `Form.cpp` constructor: **Kritik mantık hatası** — `sign_grade(sign_status)` yazılmıştı, yani constructor'a verilen grade parametresi hiç kullanılmıyor, `sign_grade` her zaman `sign_status`'un başlangıç değerine (0/false) eşitleniyordu. Bu yüzden her Form, hangi grade verilirse verilsin `GradeTooHighException` fırlatıyordu. `sign_grade(_sign_grade)` olarak düzeltildi.
- `Form.cpp`: Constructor ve copy constructor'daki initializer-list sıraları header'daki declare sırasıyla (`name, sign_status, sign_grade, sign_exec`) eşleşecek şekilde düzeltildi (`-Wreorder` hatası).
- `Form.hpp`: Dosya sonundaki `operator<<` yanlış tipte declare edilmişti (`const Bureaucrat &obj` yerine `const Form &obj` olmalıydı) — muhtemelen kopyala-yapıştır kalıntısıydı.
- `Form.cpp`: `operator<<` fonksiyonunda eksik olan `return out;` eklendi (`-Wreturn-type` hatası).
- `Bureaucrat.hpp` ↔ `Form.hpp`: Karşılıklı `#include` (circular include) derlemeyi kırıyordu; her iki header'da da tam include yerine sadece referans için gereken `class X;` forward declaration kullanıldı.
- `Bureaucrat.cpp`: Aynı initializer-sırası hatası ve `signForm` içindeki unicode "akıllı tırnak" (`’`) karakteri düz apostrofa (`'`) çevrildi.
- `Makefile`: Tamamen boştu, sıfırdan yazıldı.
- `main.cpp`: Form/`signForm`/`beSigned` hiç test edilmiyordu; başarılı imza, düşük grade ile imza reddi ve geçersiz grade ile Form oluşturma senaryolarını kapsayan gerçek testler eklendi.

## ex02 - AForm ve concrete formlar

- `AForm.hpp/cpp`: ex01'deki `Form` ile aynı bug'lar (yanlış `sign_grade` initializer, initializer sırası, yanlış `operator<<` imzası, eksik `return`) düzeltildi.
- `AForm`: `execute()` pure virtual'dı ve hiçbir ortak kontrol yoktu. Artık `execute()` base class'ta concrete bir fonksiyon: imza kontrolü (`sign_status`) ve execute-grade kontrolünü (`sign_exec`) kendisi yapıp, asıl işi yapan `protected virtual void action() const = 0;` fonksiyonunu çağırıyor. Subject'in önerdiği "daha zarif" yaklaşım — kontrol mantığı her concrete class'ta tekrarlanmıyor.
- `AForm::FormNotSignedException::what()` boş string döndürüyordu, anlamlı bir mesaj (`"the form is not signed"`) verildi.
- `Bureaucrat.hpp/cpp`: Var olmayan bir `Form.hpp`'yi include edip `Form` tipini kullanıyordu (ex02 klasöründe `Form.hpp` yok). `AForm`'a göre yeniden yazıldı; eksik olan `executeForm(AForm const &) const` fonksiyonu eklendi.
- `Bureaucrat.hpp` ↔ `AForm.hpp` arasında ex01'dekiyle aynı circular-include sorunu forward declaration ile çözüldü.
- **`ShrubberyCreationForm`, `RobotomyRequestForm`, `PresidentialPardonForm`**: `.hpp` ve `.cpp` dosyalarının hepsi tamamen boştu, sıfırdan yazıldı:
  - `ShrubberyCreationForm` (sign 145 / exec 137): `<target>_shrubbery` dosyasını oluşturup içine ASCII ağaç yazıyor.
  - `RobotomyRequestForm` (sign 72 / exec 45): "drilling noises" mesajı basıp `%50` ihtimalle başarı/başarısızlık mesajı veriyor (`std::rand()`).
  - `PresidentialPardonForm` (sign 25 / exec 5): pardon mesajı basıyor.
  - Üçü de Orthodox Canonical Form'a uygun (default/copy/assign/destructor).
- `Makefile`: Boştu, sıfırdan yazıldı.
- `main.cpp`: `AForm`/`execute`/`executeForm` hiç test edilmiyordu; imzalama, imzasız çalıştırma reddi, düşük grade ile çalıştırma reddi ve başarılı çalıştırma senaryolarını her üç concrete class için kapsayan testler eklendi.

## ex03 - Intern

Klasör baştan yoktu, sıfırdan oluşturuldu:

- ex02'den `Bureaucrat`, `AForm`, `ShrubberyCreationForm`, `RobotomyRequestForm`, `PresidentialPardonForm` dosyaları (zaten düzeltilmiş halleriyle) kopyalandı.
- `Intern.hpp/cpp`: Hiç üye değişkeni yok (subject'in istediği gibi "no name, no grade, no unique characteristics"), yine de OCF'e uyuyor (default/copy/assign/destructor).
- `Intern::makeForm(formName, target)`: Subject'in yasakladığı if/elseif/else yığını yerine, form adı → üretici fonksiyon eşlemesini tutan statik bir tablo (fonksiyon pointer'ları ile) üzerinde döngüyle arama yapıyor. Bulunca ilgili `AForm*`'u `new` ile üretip "Intern creates <form>" basıyor, bulamayınca açıklayıcı bir hata mesajı basıp `NULL` döndürüyor.
- `Makefile`: Sıfırdan yazıldı, önceki egzersizlerin tüm kaynak dosyalarını + `Intern.cpp`'yi derliyor.
- `main.cpp`: Üç geçerli form adı ve bir geçersiz form adı deneniyor; geçerli olanlar için üretim → imzalama (`signForm`) → yürütme (`executeForm`) → `delete` akışı test ediliyor, geçersiz olan için `NULL` dönüşü kontrol edilip atlanıyor (leak yok, `signForm`/`executeForm` kendi exception'larını kendi içinde yakaladığı için `delete` hiçbir zaman atlanmıyor).

## Yan not (kaza ve düzeltmesi)

`ex00/Makefile`'ı `make re` ile test ederken yukarıdaki bug yüzünden `Bureaucrat.hpp` kazayla silindi; git geçmişinden (`git show HEAD:...`) içerik okunup dosya geri yazıldı. Aynı şekilde session sırasında `.vscode/settings.json` da kayboldu (bu işlemi ben yapmadım), o da git geçmişinden geri yüklendi. `git status` şu an temiz, veri kaybı yok.

## Durum

ex00, ex01, ex02, ex03 → `c++ -Wall -Wextra -Werror -std=c++98` ile temiz derleniyor, davranışları subject'teki beklentilerle eşleşiyor.
