# CPP05 — Repetition and Exceptions

Bu modülün amacı, C++'ın **exception (istisna) mekanizmasını** ve **Orthodox
Canonical Form**'u (OCF) öğretmek. ex02'ye gelindiğinde buna **inheritance
(kalıtım)** ve **polymorphism (çok biçimlilik)** de ekleniyor. Aşağıda her
egzersizin class yapısı, her üye değişken/fonksiyonun ne işe yaradığı ve
projenin kazandırmak istediği C++ kavramları anlatılıyor.

---

## 0. Önce genel kavramlar

### Exception (istisna) mekanizması nedir, neden var?

Normal bir fonksiyon bir hata durumuyla karşılaştığında iki seçeneği vardır:
özel bir "hata" değeri döndürmek (örn. `-1`, `NULL`) ya da bir **exception
fırlatmak** (`throw`). İkincisi C++'a özgü, daha güçlü bir yöntemdir çünkü:

- Constructor'lar değer döndüremez (`int` değil), ama başarısız
  olabilirler (örn. geçersiz bir grade ile `Bureaucrat` oluşturmak). Bunu
  ifade etmenin tek yolu exception fırlatmaktır.
- Hata, onu üretebilecek her fonksiyonda tek tek kontrol edilmek zorunda
  kalınmadan, çağrı zincirinde yukarı doğru "fırlar" ve onu yakalayacak
  (`catch`) ilk yere kadar gider.

```
throw BirSeyKotu();   // "bir hata oluştu" diye bir nesne fırlatılır
```

`throw` edilen şey bir **nesnedir** (int, string, ya da özel bir sınıf
olabilir). Bu projede hepsi `std::exception`'dan türeyen özel sınıflar.

### `std::exception` ve `what()`

Standart kütüphanedeki taban exception sınıfı:

```cpp
class exception
{
public:
    virtual const char *what() const throw();
};
```

- `virtual`: Alt sınıflar bu fonksiyonu **override** edip kendi hata
  mesajlarını dönebilir. Bu projede her exception sınıfı (`GradeTooHighException`,
  `GradeTooLowException`, `FormNotSignedException`...) `std::exception`'dan
  türetilip `what()` override ediliyor:

  ```cpp
  class GradeTooHighException : public std::exception
  {
  public:
      virtual const char *what() const throw();
  };
  ```

- `const` (fonksiyonun sonundaki): Bu fonksiyon nesnenin üye değişkenlerini
  değiştirmez. `catch (const std::exception &e)` gibi const referanslar
  üzerinden çağrılabilmesi için `what()`'in const olması **zorunludur**.
- `throw()` (fonksiyonun sonundaki, eski C++98 sözdizimi): "Bu fonksiyon
  hiçbir exception fırlatmaz" garantisi verir (C++11'deki `noexcept`'in
  atası). `what()` bir hata mesajı döndürürken kendisi hata fırlatırsa çok
  kötü olurdu, o yüzden bu garanti önemlidir.
- Neden `const char*` ve `std::string` değil? Çünkü CPP05'te henüz STL
  container/string manipülasyonu güvenli sayılmıyor exception bağlamında;
  ayrıca imza `std::exception`'ın kendisinden geliyor, değiştirilemez —
  override eden her `what()` aynı imzayı kullanmak zorunda.

### `try` / `catch` nasıl çalışır?

```cpp
try
{
    Bureaucrat b("Test", 0);   // constructor içinde throw ediliyor
    std::cout << "buraya hiç gelinmez" << std::endl;
}
catch (const std::exception &e)
{
    std::cerr << e.what() << std::endl;
}
```

- `try` bloğu içindeki kod normal çalışır. Bir `throw` gerçekleştiği anda,
  `try` bloğunun geri kalanı **atlanır** ve kontrol doğrudan `catch`'e
  geçer.
- `catch (const std::exception &e)` — burada `std::exception`'a **referans**
  ile yakalama yapılıyor. Bu, **polymorphic catch** denen şeyi sağlar:
  `GradeTooHighException`, `GradeTooLowException`, `FormNotSignedException`
  gibi hepsi `std::exception`'dan türediği için, hepsini **tek bir catch**
  bloğu yakalayabilir. `e.what()` çağrıldığında, `virtual` sayesinde her
  zaman doğru (fırlatılan gerçek tipin) mesajı basılır — buna **dynamic
  dispatch** denir.
- Referans yerine değer ile yakalamak (`catch (std::exception e)`) **object
  slicing**'e yol açar (alt sınıfın kendine özgü kısmı kesilip atılır);
  bu yüzden hep `&` ile yakalanır.
- Yakalanmayan bir exception programı `std::terminate()` ile çökertir. Bu
  yüzden her `throw` edebilecek çağrının bir `try/catch` içinde olması
  gerekir.

### Orthodox Canonical Form (OCF)

Modül 02'den itibaren zorunlu tutulan kalıp — bir sınıfın şu 4 üyeye sahip
olması:

1. **Default constructor** — parametresiz, nesneyi "boş"/varsayılan bir
   duruma getirir.
2. **Copy constructor** — `ClassName(const ClassName &other)`, var olan bir
   nesneden yenisini üretir.
3. **Copy assignment operator** — `ClassName &operator=(const ClassName &other)`,
   var olan bir nesneye başka bir nesnenin içeriğini kopyalar.
4. **Destructor** — nesne kapsam dışına çıktığında/`delete` edildiğinde
   çalışır, kaynakları temizler.

Bu proje boyunca her sınıf (exception sınıfları hariç — subject bunu
istisna tutuyor) bu dördünü tanımlıyor, çoğu da işlemin hangi aşamada
olduğunu görebilmeniz için `std::cout` ile "... called" mesajı basıyor.

---

## 1. ex00 — `Bureaucrat`

**Amaç:** Basit bir sınıf üzerinden constructor'da exception fırlatmayı,
`const` üye kullanımını ve OCF'i öğretmek.

### Üye değişkenler (`Bureaucrat.hpp`)

```cpp
private:
    int grade;              // 1 (en yüksek) - 150 (en düşük)
    const std::string name; // bir kez set edilir, asla değişmez
```

- `name` neden `const`? Bir bürokratın adı gerçek hayatta değişmez — bunu
  dil seviyesinde garanti altına almak için `const` yapılıyor. Sonucu:
  `name` sadece **initializer list**'te set edilebilir
  (`Bureaucrat::Bureaucrat(...) : name(_name) { ... }`), constructor
  gövdesinde (`{ }` içinde) `this->name = ...` gibi bir atama **derlenmez**
  (const üyeler atanamaz, sadece initialize edilebilir).
- `grade` const değil çünkü `incrementGrade()`/`decrementGrade()` ile
  değişebilmesi gerekiyor.
- **Initializer list sırası önemlidir**: Üyeler, class içinde **declare
  edildikleri sırayla** initialize edilir — initializer list'te yazdığınız
  sıra değil! Bu projede bulunan ve düzeltilen bug tam olarak buydu:
  `grade` header'da `name`'den önce declare edilmişken, initializer list'te
  `name(...), grade(...)` şeklinde ters yazılmıştı. Derleyici bunu
  `-Wreorder` uyarısı ile tespit eder (bu projede `-Werror` ile hataya
  dönüşür).

### Fonksiyonlar

| Fonksiyon | Ne işe yarar |
|---|---|
| `Bureaucrat()` | Default constructor. OCF gereği var; `name="none"`, `grade=150` ile başlatır. |
| `Bureaucrat(name, grade)` | Asıl constructor. `grade` sınırlarını kontrol eder, aşımda `throw` eder. |
| `Bureaucrat(const Bureaucrat &copy)` | Copy constructor. `name` const olduğu için initializer list'te kopyalanır. |
| `operator=(const Bureaucrat &copy)` | Copy assignment. **Sadece `grade` kopyalanır** — `name` const olduğu için zaten kopyalanamaz (dil kısıtı, bug değil). |
| `~Bureaucrat()` | Destructor, "yok edildi" mesajı basar. |
| `getName()` / `getGrade()` | Read-only erişim — üyeler `private` olduğu için dışarıdan doğrudan erişilemez, bu **encapsulation**'ın (kapsülleme) gereği. |
| `incrementGrade()` / `decrementGrade()` | Grade'i 1 değiştirir. **Dikkat: isimler kafa karıştırıcı olabilir** — grade 1 "en yüksek" olduğu için `incrementGrade()` sayıyı **azaltır** (`--grade`), `decrementGrade()` **artırır** (`++grade`). Sınır aşılırsa (`grade<=1` ya da `grade>=150`) constructor'la aynı exception'ları fırlatır. |

### Exception sınıfları

```cpp
class GradeTooHighException : public std::exception { ... };  // grade < 1 istenirse
class GradeTooLowException  : public std::exception { ... };  // grade > 150 istenirse
```

Bunlar `Bureaucrat`'ın **nested class**'ları (`Bureaucrat::GradeTooHighException`
gibi çağrılıyor) — mantıksal olarak sadece `Bureaucrat` ile ilgili
oldukları için sınıfın içine gömülmüşler, global namespace'i kirletmiyorlar.

### `operator<<` overload

```cpp
std::ostream &operator<<(std::ostream &out, const Bureaucrat &obj);
```

Bu bir **üye fonksiyon değil, serbest (free) fonksiyondur** — çünkü
`std::cout << bureaucrat` yazıldığında sol taraf (`std::cout`) zaten
`std::ostream`'in kendisi, `Bureaucrat`'ın değil. Üye fonksiyon olsaydı
`bureaucrat << std::cout` yazmak gerekirdi. `out` referans olarak alınıp
**referans olarak döndürülür** (`return out;`) ki
`std::cout << a << b << c` gibi **zincirleme** kullanılabilsin.

### `main.cpp`'deki try/catch testleri

Dört ayrı senaryo test ediliyor: geçerli grade ile oluşturup
increment/decrement sınırına kadar götürmek, aralık dışı grade (`151` ve
`0`) ile constructor'ı patlatmak. Her biri ayrı bir `try/catch` içinde,
böylece bir testin fırlattığı exception diğerlerini durdurmuyor.

---

## 2. ex01 — `Form`

**Amaç:** İki sınıfın birbirini kullanmasını (`Bureaucrat` bir `Form`'u
imzalıyor), exception'ların bir fonksiyondan diğerine nasıl "iletildiğini"
ve **circular include** (döngüsel include) sorununu öğretmek.

### Üye değişkenler (`Form.hpp`)

```cpp
private:
    const std::string name;    // form adı, sabit
    bool sign_status;          // imzalı mı? constructor'da hep false
    const int sign_grade;      // imzalamak için gereken min. grade
    const int sign_exec;       // (ex01'de kullanılmıyor, ex02'de execute için)
```

`sign_grade` ve `sign_exec` neden `const`? Bir formun "imza için gereken
grade"si, form basıldıktan sonra değişmez — bu iş kuralını `const` ile
derleyiciye kabul ettiriyoruz.

### `beSigned()` — asıl mantık

```cpp
void Form::beSigned(const Bureaucrat &approver)
{
    if (approver.getGrade() > this->sign_grade)
        throw Form::GradeTooLowException();
    this->sign_status = true;
}
```

Grade 1 "en yüksek" olduğu için, `approver.getGrade() > sign_grade`
kontrolü aslında "bürokratın rütbe **sayısı**, gereken rütbe sayısından
**büyükse** (yani rütbesi gerekenden **düşükse**)" anlamına gelir. Örnek:
form `sign_grade=50` istiyor, bürokrat `grade=150` (stajyer) → `150 > 50`
→ reddedilir. Bürokrat `grade=1` (patron) → `1 > 50` yanlış → imzalanır.

### `Bureaucrat::signForm()` — iki sınıf arasındaki köprü

```cpp
void Bureaucrat::signForm(Form &form)
{
    try
    {
        form.beSigned(*this);
        std::cout << this->name << " signed " << form.getName() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << this->name << " couldn't sign " << form.getName()
                   << " because " << e.what() << std::endl;
    }
}
```

Bu, exception mekanizmasının tipik kullanım şeklini gösteriyor: `beSigned()`
başarısız olursa **exception fırlatır**, kendisi hiçbir hata mesajı
basmaz — mesaj basma sorumluluğu onu **çağıran** yere (`signForm`) ait.
Böylece `beSigned()` hem `signForm()` hem farklı bir bağlamdan (örn.
doğrudan test kodundan) çağrılabilir, her çağıran hatayı kendi istediği
şekilde ele alabilir. Bu **ayrım (separation of concerns)** exception'ların
en büyük avantajlarından biridir.

`*this` neden geçiliyor? `Form::beSigned`, formu imzalayan bürokratın
kim olduğunu (`getGrade()` için) bilmek zorunda; `Bureaucrat::signForm()`
içinde "ben" demenin yolu `*this`'tir (kendi nesnesine referans).

### Circular include problemi ve forward declaration

`Bureaucrat`, bir `Form&` parametresi alıyor (`signForm`); `Form`, bir
`Bureaucrat&` parametresi alıyor (`beSigned`). İkisi de birbirinin
header'ını `#include` etseydi:

```
Bureaucrat.hpp  -->  #include "Form.hpp"
Form.hpp        -->  #include "Bureaucrat.hpp"   // include guard nedeniyle atlanır!
```

`main.cpp` önce `Bureaucrat.hpp`'yi include ettiğinde, o da `Form.hpp`'yi
include eder; `Form.hpp` da `Bureaucrat.hpp`'yi include etmeye çalışır ama
include guard (`#ifndef BUREAUCRAT_HPP`) zaten aktif olduğu için bu include
**hiçbir şey yapmaz** — ve `Form.hpp`, henüz tanımlanmamış bir `Bureaucrat`
tipini kullanmaya çalışır → derleme hatası.

Çözüm: Bir tipe sadece **referans/pointer** olarak ihtiyacınız varsa
(üye fonksiyonun tam tanımını görmenize gerek yoksa), tam `#include`
yerine **forward declaration** yeterlidir:

```cpp
// Form.hpp içinde
class Bureaucrat;   // "böyle bir sınıf var, detayını şimdi bilmeme gerek yok"
```

Derleyici referans/pointer parametreleri için tipin boyutunu bilmek
zorunda değildir, sadece "böyle bir isim var" bilmesi yeterlidir. Gerçek
tanıma ihtiyaç duyulan yer (`.cpp` dosyaları, üye fonksiyon gövdeleri)
zaten kendi `#include`'unu yapıyor.

---

## 3. ex02 — `AForm` ve somut form sınıfları

**Amaç:** **Abstract class**, **inheritance** ve **polymorphism**
öğretmek — CPP04'te başlayan konunun exception'larla birleşimi.

### Abstract class nedir? (`AForm`)

```cpp
protected:
    virtual void action() const = 0;
```

`= 0` bir fonksiyonu **pure virtual** yapar: `AForm`'un kendisi bu
fonksiyonun **hiçbir gövdesini** vermez, sadece "her alt sınıf bunu
kendi şekilde uygulamak zorunda" der. Bir sınıfın en az bir pure virtual
fonksiyonu varsa o sınıf **abstract** olur — yani **doğrudan
nesnelenemez** (`AForm f;` derlenmez), sadece alt sınıfları üzerinden
kullanılabilir. Bu, subject'in "Form artık soyut olmalı" isteğinin
karşılığıdır.

### `execute()` / `action()` ayrımı — Template Method deseni

Subject şunu söylüyor: "İmza ve grade kontrolünü her concrete class'ta
tekrar tekrar yazabilirsin, ya da base class'ta bir kere yazıp asıl işi
yapan fonksiyonu çağırabilirsin — ikincisi daha zarif." Bu projede
ikinci yol seçildi:

```cpp
// AForm.cpp — base class'ta, HERKESE ORTAK kontrol
void AForm::execute(Bureaucrat const &executor) const
{
    if (!this->sign_status)
        throw AForm::FormNotSignedException();
    if (executor.getGrade() > this->sign_exec)
        throw AForm::GradeTooLowException();
    this->action();   // <-- asıl işi yapan kısmı alt sınıfa devret
}
```

`execute()` **virtual değil** (override edilmesine gerek yok, herkes için
aynı), ama içeride çağırdığı `action()` **pure virtual**. Buna
**Template Method** tasarım deseni denir: iskelet (kontrol sırası) base
class'ta sabit, değişen tek kısım (`action`) alt sınıfa bırakılıyor. Bu
sayede 3 concrete class'ın hiçbiri "imzalı mı, grade yeterli mi" kontrolünü
tekrar yazmak zorunda kalmıyor.

### Polymorphism (çok biçimlilik) burada nasıl işliyor?

`Bureaucrat::executeForm(AForm const &form)` bir `AForm` referansı alıyor
— ama çalışma zamanında bu aslında bir `ShrubberyCreationForm`,
`RobotomyRequestForm` ya da `PresidentialPardonForm` olabilir.
`form.execute(*this)` çağrıldığında, `execute()` içindeki `this->action()`
çağrısı **hangi somut sınıfın `action()`'ı olduğunu çalışma zamanında**
(runtime'da) çözer — buna **dynamic dispatch** denir ve `virtual`
anahtar kelimesi olmadan mümkün değildir. Bu, `if/else if` zincirleriyle
"hangi form bu, ona göre farklı davran" yazmak yerine kullanılan asıl
OOP çözümüdür — subject'in "if/else if yığınından kaçının" uyarısı tam
olarak bunu kastediyor.

### `virtual` destructor neden gerekli?

```cpp
virtual ~AForm();
```

Eğer bir `AForm*` (ya da referans) üzerinden bir `ShrubberyCreationForm`
nesnesi `delete` edilirse (ex03'te `Intern::makeForm()` tam olarak bir
`AForm*` döndürüyor), destructor `virtual` değilse **sadece `AForm`'un**
destructor'ı çalışır, `ShrubberyCreationForm`'un kendi destructor'ı hiç
çağrılmaz — bu bir **undefined behavior / memory leak** kaynağıdır. Base
class'ı polymorphic (yani en az bir `virtual` fonksiyonu olan) her sınıfın
destructor'ı da `virtual` olmalıdır; bu yüzden `AForm`'a eklendi.

### Üç concrete sınıf

Her biri `AForm`'dan **public inheritance** ile türüyor
(`class X : public AForm`), kendi `sign_grade`/`sign_exec` değerlerini
`AForm`'un constructor'ına iletiyor, kendi `target` (hedef) üyesini
tutuyor (bu `AForm`'da yok, çünkü sadece somut formlara özgü) ve
`action()`'ı override ediyor:

| Sınıf | sign / exec | `action()` ne yapıyor |
|---|---|---|
| `ShrubberyCreationForm` | 145 / 137 | `std::ofstream` ile `<target>_shrubbery` dosyası açıp içine ASCII ağaç yazıyor. |
| `RobotomyRequestForm` | 72 / 45 | "Delme sesi" mesajı basıyor, `std::rand() % 2` ile %50 ihtimalle başarı/başarısızlık mesajı basıyor. |
| `PresidentialPardonForm` | 25 / 5 | Doğrudan af mesajı basıyor. |

`sign`/`exec` değerleri ne kadar **küçükse**, o formu imzalamak/yürütmek
o kadar **yüksek rütbe** ister (grade 1 en yüksek olduğu için). Yani
"başkanlık affı" (5) çok daha üst düzey yetki isterken, "çalı dikme
izni" (137) neredeyse herkesin yürütebileceği kadar düşük yetki ister —
bu subject'in bilinçli bir mizahi tasarımı (bürokraside önemsiz işler
için daha çok imza/rütbe gerekir).

### `Bureaucrat::executeForm()`

`signForm()` ile birebir aynı desende: `try` içinde `form.execute(*this)`
çağrılır, başarılıysa "yürütüldü" mesajı, exception yakalanırsa
`e.what()` ile açıklayıcı hata mesajı basılır.

---

## 4. ex03 — `Intern`

**Amaç:** Bir sınıfı **factory (üretici)** olarak kullanmayı, **fonksiyon
pointer'larını**, `if/else if` yığınından kaçınmayı ve `new`/`delete` ile
**sahiplik (ownership)** devrini öğretmek.

### Neden `Intern`'in hiç üye değişkeni yok?

```cpp
class Intern
{
public:
    Intern();
    Intern(const Intern &copy);
    Intern &operator=(const Intern &copy);
    ~Intern();

    AForm *makeForm(const std::string &formName, const std::string &target) const;
};
```

Subject açıkça söylüyor: "intern'in adı yok, rütbesi yok, hiçbir özelliği
yok — bürokratların umursadığı tek şey işini yapması." Yani `Intern` burada
bir **veri taşıyıcısı değil, saf bir davranış (fabrika) sağlayıcısı**.
Bu yüzden `private` bölümü bile yok, hiçbir üyesi yok. OCF gereği yine de
default/copy constructor, `operator=` ve destructor tanımlanıyor —
üye olmadığı için gövdeleri boş, ama "her sınıf OCF'e uyar" kuralı
istisnasız uygulanıyor. Copy constructor/`operator=` parametresi
kullanılmadığı için `-Wextra`'nın "unused parameter" uyarısını
susturmak amacıyla `(void)copy;` yazıldı — parametreyi görmezden geldiğimizi
derleyiciye açıkça belirtmenin standart yolu budur.

### `makeForm()` — neden if/else if yerine bir tablo?

Subject şunu özellikle vurguluyor: *"You must avoid unreadable and messy
solutions such as an excessive if/elseif/else structure. This kind of
approach will not be accepted."* Naif çözüm şöyle olurdu:

```cpp
// KÖTÜ ÖRNEK — yapılmadı
if (formName == "shrubbery creation")
    return new ShrubberyCreationForm(target);
else if (formName == "robotomy request")
    return new RobotomyRequestForm(target);
else if (formName == "presidential pardon")
    return new PresidentialPardonForm(target);
else
    ...
```

Yeni bir form türü eklendikçe bu zincir uzar, okunması ve test edilmesi
zorlaşır. Bunun yerine `Intern.cpp`'de **isim → üretici fonksiyon**
eşlemesini tutan statik bir tablo kullanıldı:

```cpp
typedef AForm *(*FormFactory)(const std::string &);

struct FormEntry
{
    std::string name;
    FormFactory factory;
};

static const FormEntry forms[] = {
    { "shrubbery creation", &createShrubbery },
    { "robotomy request",   &createRobotomy },
    { "presidential pardon", &createPardon }
};

for (size_t i = 0; i < formsCount; ++i)
{
    if (forms[i].name == formName)
    {
        std::cout << "Intern creates " << formName << std::endl;
        return forms[i].factory(target);
    }
}
std::cerr << "Error: form \"" << formName << "\" does not exist" << std::endl;
return NULL;
```

- `typedef AForm *(*FormFactory)(const std::string &);` — bir **fonksiyon
  pointer** tipi tanımlıyor: "bir `std::string` alıp `AForm*` döndüren
  fonksiyon". `createShrubbery`, `createRobotomy`, `createPardon` gibi
  `Intern.cpp` içinde tanımlı `static` (yalnızca bu `.cpp`'ye özel,
  dışarıya sızmayan) yardımcı fonksiyonlar bu imzaya uyuyor, her biri
  ilgili concrete sınıfı `new` ile üretip döndürüyor.
- Yeni bir form eklemek istediğinizde tek yapmanız gereken tabloya bir
  satır eklemek — kontrol mantığının (döngü) kendisine hiç dokunmuyorsunuz.
  Bu, `if/else if` zincirinin **açık/kapalı prensibi (open/closed
  principle)** ihlaline karşı klasik bir çözümdür.
- `formsCount`, `sizeof(forms) / sizeof(forms[0])` ile hesaplanıyor —
  dizinin eleman sayısını C++98'de (STL container olmadan) almanın
  standart yolu budur.
- Tablo `static const` olarak fonksiyon içinde tanımlandığı için sadece
  **bir kez** oluşturulur (her `makeForm()` çağrısında yeniden değil),
  global scope'u da kirletmez.

### `new` ve sahiplik (ownership) devri

`makeForm()` bir `AForm*` **döndürüyor**, nesneyi kendisi silmiyor — bu
nesnenin sahipliğini **çağırana devrettiği** anlamına gelir. `main.cpp`
bunu böyle kullanıyor:

```cpp
AForm *form = intern.makeForm(formNames[i], "Bender");
if (form == NULL)
    continue;
boss.signForm(*form);
boss.executeForm(*form);
delete form;   // sahipliği aldık, temizlemek de bize düşüyor
```

`makeForm()` `NULL` döndürebildiği için (form adı tanınmazsa), her
çağrıdan sonra `NULL` kontrolü yapmak **zorunludur** — kontrol edilmeden
`*form` yapmak (null pointer dereference) programı çökertir.

Burada ex02'de eklenen **`virtual ~AForm()`** devreye giriyor: `form`
değişkeninin **statik tipi** `AForm*`'dur ama **gerçek (dinamik) tipi**
çalışma zamanında `ShrubberyCreationForm`, `RobotomyRequestForm` ya da
`PresidentialPardonForm` olabilir. `delete form;` çağrıldığında, destructor
`virtual` olduğu için doğru (gerçek tipin) destructor'ı çalışır — önce
`ShrubberyCreationForm::~ShrubberyCreationForm()`, sonra otomatik olarak
`AForm::~AForm()`. Destructor `virtual` olmasaydı sadece `AForm`'un
destructor'ı çalışır, bu örnekte pratik bir fark yaratmasa da (concrete
sınıfların destructor'ları da boş) **genel kural olarak memory leak/UB
riski** taşırdı — subject'in "C++'ta da memory leak olur, dikkat edin"
uyarısının tam karşılığı budur.

### `main.cpp`'deki test döngüsü

Dört form adı deneniyor: üç geçerlisi (`"shrubbery creation"`,
`"robotomy request"`, `"presidential pardon"`) ve bir geçersizi
(`"coffee making"`). Geçerli olanlar için tam akış test ediliyor:
üretim → imzalama (`Bureaucrat::signForm`) → yürütme
(`Bureaucrat::executeForm`) → `delete`. Geçersiz olan için `makeForm()`
`NULL` döner, döngü `continue` ile bir sonraki forma geçer — hiçbir
`delete`/kullanım denenmez.

---

## 5. Bu projede öğrenilen C++ kavramlarının özeti

- **`throw` / `try` / `catch`**: Hataları normal akıştan ayırarak yönetme.
- **`std::exception` ve `virtual what()`**: Ortak bir exception arayüzü,
  tek `catch` bloğuyla farklı hata tiplerini yakalayabilme (polymorphic
  catch).
- **`const` doğruluğu (const correctness)**: Değişmemesi gereken üyeleri
  (`name`, `sign_grade`...) ve değiştirmeyen fonksiyonları (`getGrade() const`)
  derleyiciye işaretleyip hataları derleme zamanında yakalamak.
- **Initializer list sırası**: Üyelerin, initializer list'teki değil,
  **class içinde declare edildikleri sırayla** kurulduğu.
- **Orthodox Canonical Form**: Her sınıfın default/copy constructor,
  copy assignment, destructor dörtlüsüyle güvenli kopyalanabilir/atanabilir
  olması.
- **Encapsulation**: `private` üyeler + `public` getter'lar ile bir
  sınıfın iç halini dışarıdan doğrudan bozulamaz hale getirmek.
- **Operator overloading**: `operator<<`'ı serbest fonksiyon olarak
  tanımlayıp `std::cout << nesne` sözdizimini kendi tiplerimize
  taşımak.
- **Forward declaration & circular include**: İki sınıfın birbirine
  referansla ihtiyaç duyduğu (ama tam tanımına ihtiyaç duymadığı)
  durumlarda `#include` yerine `class X;` kullanmak.
- **Abstract class / pure virtual (`= 0`)**: Bir arayüzü zorunlu kılıp
  doğrudan nesnelenmeyi engellemek.
- **Inheritance (`: public AForm`)**: Ortak veri/davranışı base class'ta
  toplayıp alt sınıflarla özelleştirmek.
- **Polymorphism / dynamic dispatch**: Base class referans/pointer'ı
  üzerinden çalışma zamanında doğru alt sınıf davranışının çağrılması.
- **Template Method deseni**: Ortak akışı (`execute`) base class'ta sabit
  tutup, değişen tek adımı (`action`) alt sınıfa bırakmak — `if/else`
  zincirlerinden kaçınmanın OOP yolu.
- **`virtual` destructor**: Polymorphic silme (`delete` bir base class
  pointer'ı üzerinden) sırasında doğru destructor zincirinin çalışmasını
  garanti etmek.
- **Factory pattern**: Bir sınıfın (`Intern`), hangi somut tipin
  üretileceğine çalışma zamanında karar verip base class pointer'ı
  (`AForm*`) döndürmesi — çağıran taraf hangi concrete sınıfla
  uğraştığını bilmek zorunda kalmaz.
- **Fonksiyon pointer'ları (`typedef ... (*FormFactory)(...)`)**: Bir
  fonksiyonu veri gibi (bir dizinin/tablonun elemanı olarak) taşıyıp
  çalışma zamanında çağırabilmek — `if/else if` zincirini bir arama
  tablosuna indirgemenin yolu.
- **Sahiplik (ownership) devri ve manuel bellek yönetimi**: `new` ile
  üretilen bir nesnenin sorumluluğunun, onu döndüren fonksiyondan onu
  çağıran koda geçmesi; `NULL` kontrolü ve `delete` ile bu sorumluluğun
  eksiksiz yerine getirilmesi (memory leak'ten kaçınma).
