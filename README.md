# ACS_IHW4
# АВС ИДЗ№4 ВАРИАНТ№19
БПИ216 Дюгай Андрей Владимирович

19. Задача о магазине - 2 (забывчивые покупатели). В магазине работают
два отдела, каждый отдел обладает уникальным ассортиментом. В каждом
отделе работает один продавец. В магазин ходят исключительно забывчивые
покупатели, поэтому каждый покупатель носит с собой список товаров, которые желает купить. Покупатель приобретает товары точно в том порядке, в
каком они записаны в его списке. При этом товары в списке расположены в
случайном порядке, что заставляет покупателя переходить от отдела к отделу, если это требуется для совершения покупок. Продавец может обслужить
только одного покупателя за раз. Покупатель, вставший в очередь, засыпает
пока не дойдет до продавца. Продавец засыпает, если в его отделе нет покупателей, и просыпается, если появится хотя бы один. Создать многопоточное приложение, моделирующее работу магазина в течение рабочего
дня.

tests.md - файл с тестами

Ход работы:

"Клиенты и серверы" - архитектура, которая просто "напрашивается" после прочтения условия задачи, очевидно, что серверами будут являться продавцы, а клиентами - покупатели. "Клиентский поток запрашивает сервер и ждет ответа. Серверный поток ожидает запроса от клиента, затем действует в соответствии с поступившим запросом."
Описание из лекционной презентации по АВС. В моём случае алгоритм будет следующим:

ПОКУПАТЕЛИ:
1) составление списка покупок для каждого покупателя
2) покупатель №i начинает "идти по списку", обращается к соответствующему продавцу ("запрашивает товар")
3) ждёт и получает ответ от продавца ("получает товар")
4) пункт 2, если не были куплены все товары из списка

ПРОДАВЦЫ:
1) получают запрос от покупателя №i
2) "думают, ищут товар" (sleep())
3) "выдают товар покупателю №i" (возвращают название товара)

Пока ПОКУПАТЕЛИ и ПРОДАВЦЫ ждут своей очереди/ожидают покупателей - они ничего не делают, как бы "спят"

ПРИМЕР (!пример упрощён! в программе 4 покупателя и у продавцов немного другие товары!):

0) у продавцов заранее есть список уникальных товаров (1 - яблоки, груши; 2 - апельсины, мандарины)
1) с консоли вводятся списки покупателей (1 - яблоки, груши, апельсины; 2 - яблоки, мандарины, апельсины)
2) покупатель 1 и покупатель 2 делют запрос продавцу 1 на яблоки (потому что он ими торгует)
3) продавец 1 получает запрос, допустим первым пришел запрос от покупателя 1, начинает его обрабатывать (в это время все остальные бездействуют)
4) продавец 1 отвечает покупателю 1 (который сразу делает запрос на груши продавцу 1) и переходит к обработке запроса покупателя 2
5) продавец 1 отвечает покупателю 2 (который сразу делает запрос на мандарины продавцу 2, т.к. продавец 2 был свободен, он сразу начнет обработку запроса) и переходит к обработке запроса покупателя 1
6) продавец 1 отвечает покупателю 1/продавец 2 отвечает покупателю 2, оба покупателя делают запрос продавцу 2 на апельсины
7) продавец обрабатывает запросы и отвечает по очереди покупателям 1 и 2


должна быть обработка неправильного ввода

пояснить за ввод и вывод

"Я на лекции говрил, что есть PARALLELISM и CONCURRENCY. Также говорил, что в задачах идет речь о втором виде параллелизма. О взаимодействии ресурсов."
Слова Александра Ивановича из чата "АВС | Официальный" (https://t.me/c/1895583371/1518)

"Concurrency is about multiple tasks which start, run, and complete in overlapping time periods, in no specific order." https://freecontent.manning.com/concurrency-vs-parallelism/

Таким образом нам не особо важно, что соблюдается "честная очередь", т.е. покупатели могут получить ответ от продавца раньше, даже если запрос был позже

Описаны входные данные программы, включающие вариативные
диапазоны, возможные при многократных запусках. (НЕСКОЛЬКО РАЗНЫХ ТЕСТОВ + РЕЗЫ)

Ввод данных в приложение реализован с консоли.

КОММЕНТАРИИ В КОДЕ

накину мьтех на функцию продавца (уснуть на время и вернуть название продукта)
и мьютехи пока я вывожу действие
