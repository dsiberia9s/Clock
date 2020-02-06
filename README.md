# **![](https://habrastorage.org/webt/z4/i9/mk/z4i9mk0fqopeehdivctzq2srewe.jpeg)**

**[по-русски]**

Синхронизация времени по стандарту **JJY**. Используется встроенный микрофон (**GPIO 34**). На частоте **13 кГц** мобильное приложение **JJY Simulator** вещает дату и время с помощью динамика. **M5Stack** производит преобразование Фурье, далее измеряет длительность сигнала заданной частоты, затем накапливает полученные биты и извлекает информацию о времени. Подробное описание стандарта приведено на сайте https://en.wikipedia.org/wiki/JJY . После успешного накопления полезных битов, происходит синхронизация программых часов, осуществляется перевод стрелок.

**[English]**

Time synchronization according to **JJY** standard. The built-in microphone (**GPIO 34**) is used. At a frequency of **13 kHz** mobile application **JJY Simulator** broadcasts the date and time using the speaker. **M5Stack** performs the Fourier transform, then measures the duration of the signal of a given frequency, then it accumulates the received bits and extracts time information. A detailed description of the standard is available at https://en.wikipedia.org/wiki/JJY. After the successful accumulation of useful bits, the software clock is synchronized, the hands are translated.