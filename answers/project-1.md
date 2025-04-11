# سوال ۱

وظیفهٔ داکر اینه که صرفا یک محیط با dependencyهای لازم برای build و اجرای پروژه رو برای ما بالا بیاره. 

داکر در واقع وظیفه‌ش virtualize کردن در سطح OSئه، و کاری که می‌کنه اینه که یک محیط vistualize شده بالا میاره که نمی‌تونه روی باقی فایل‌ها تاثیر بذاره. این‌جا ما از یک image داکر استفاده می‌کنیم که کامپایلر لازم برای build کردن پروژه و qemu رو توی خودش داره. البته ما این container رو یک جا هم به خود file systemمون با آرگیومنت ‍‍`v /Users/armaho/projects/xv6-riscv:/home/xv6/xv6-riscv-` وصل می‌کنیم.

وظیفهٔ qemu اما متفاوته. qemu در سطح hardware مجازی‌سازی می‌کنه و به ما اجازه می‌ده osهای مختلف رو بدون این که روی سیستم‌مون نصب کنیم به شکل virtualize شده داشته باشیم. در واقع qemu چیزی شبیه VMWareئه که به ما اجازه می‌ده riscv (که image اون بعد از make توی fs.img هست) رو اجرا کنیم.

# سوال ۲

‏file descriptorها، intهایی هستن که index یک فایل باز رو توی file descriptor table نشون می‌دن. هر entry توی این table، نشان دهندهٔ یک I/Oی باز برای سیستم ماست. وقتی system callهای مرتبط با I/O رو صدا می‌زنیم، راه این که با OS ارتباط بگیریم این file descriptorهان.

# سوال ۳

سیستم کال exec وظیفهٔ این رو داره که image یک executable جدید رو به جای process فعلی توی مموری load کنه. 

خوبی ادغام نکردن اون با fork، اینه که ما می‌تونیم قبل از صدا زدن exec برای اجرای فرزند آماده بشیم. مثلا pipe کردن و redirect از همین مزیت استفاده می‌کنه و بعد از fork و قبل از exec، فایل دیسکریپتورهای 0 یا 1 رو می‌بنده و به جای اون‌ها فایل‌ دیکسریپتورهای جدیدی ایجاد می‌کنه تا ورودی و خروجی پراسس child بدون اطلاع خودش تغییر کنه. به نظرم خود riscv این رو بهتر توصیف می‌کنه:

> Now it should be clear why it is helpful that fork and exec are separate calls: between the two, the shell has a chance to redirect the child’s I/O without disturbing the I/O setup of the main shell. One could instead imagine a hypothetical combined forkexec system call, but the options for doing I/O redirection with such a call seem awkward. The shell could modify its own I/O setup before calling forkexec (and then un-do those modifications); or forkexec could take instructions for I/O redirection as arguments; or (least attractively) every program like cat could be taught to do its own I/O redirection.

# سوال ۴

چالش اصلی خود کدبیس riscv نبود. راستش نه به لحاظ مفهومی، ولی format کدهاشون و تمیزی‌ش و این‌ها یه خورده عجیب به نظر می‌رسید. یا این که فرآیند deploy و runشون با هم بود و نمی‌شد جداشون کرد راحت.

به نظرم این کد رو می‌شد تمیزتر و بهتر هم نوشت.
