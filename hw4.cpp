#include <iostream>
#include <pthread.h>
#include <set>
#include <list>
#include <unistd.h>
#include <string>

// mutex-ы для работы программы
pthread_mutex_t mutex_seller1;
pthread_mutex_t mutex_seller2;
pthread_mutex_t mutex_console_printing;
pthread_mutex_t mutex_starting;
// Множество продуктов продавца 1
std::set<std::string> products_seller1;
// Множество продуктов продавца 2
std::set<std::string> products_seller2;
// Флаги для распределения задач между потоками
bool customer1_in_the_store = false;
bool customer2_in_the_store = false;
bool customer3_in_the_store = false;
// Списки покупок
std::list<std::string> shopping_list1;
std::list<std::string> shopping_list2;
std::list<std::string> shopping_list3;
std::list<std::string> shopping_list4;

// Функция для вывода строки
void print(const std::string& string) {
    pthread_mutex_lock(&mutex_console_printing);
    std::cout << string;
    pthread_mutex_unlock(&mutex_console_printing);
}

// Функция для обращения к продавцу 1
std::string ask_seller1(std::string product, const std::string& customer_number) {
    // Блокируем mutex, чтобы к продавцу 1 мог обратиться ТОЛЬКО 1 покупатель в одно время
    pthread_mutex_lock(&mutex_seller1);
    print("продавец 1 обрабатывает заказ на товар '" + product +
                         "' от покупателя " + customer_number + "\n");
    // "Обработка запроса"
    sleep(2);
    // Разблокируем mutex
    pthread_mutex_unlock(&mutex_seller1);
    return product;
}

// Функция для обращения к продавцу 2
std::string ask_seller2(std::string product, const std::string& customer_number) {
    // Блокируем mutex, чтобы к продавцу 2 мог обратиться ТОЛЬКО 1 покупатель в одно время
    pthread_mutex_lock(&mutex_seller2);
    print("продавец 2 обрабатывает заказ на товар '" + product +
                         "' от покупателя " + customer_number + "\n");
    // "Обработка запроса"
    sleep(2);
    // Разблокируем mutex
    pthread_mutex_unlock(&mutex_seller2);
    return product;
}

// Метод для прохода по списку покупок
void* go_through_the_shopping_list(void* ignored) {
    std::string customer_number;
    std::list<std::string> shopping_list;
    // Поток становится покупателем, который ещё не начал покупки
    pthread_mutex_lock(&mutex_starting);
    if (!customer1_in_the_store) {
        customer1_in_the_store = true;
        customer_number = "1";
        shopping_list = shopping_list1;
    } else {
        if (!customer2_in_the_store) {
            customer2_in_the_store = true;
            customer_number = "2";
            shopping_list = shopping_list2;
        } else {
            if (!customer3_in_the_store) {
                customer3_in_the_store = true;
                customer_number = "3";
                shopping_list = shopping_list3;
            } else {
                customer_number = "4";
                shopping_list = shopping_list4;
            }
        }
    }
    print("покупатель " + customer_number + " вошёл на рынок\n");
    pthread_mutex_unlock(&mutex_starting);
    std::string seller_number;
    std::string gotten_product;
    for (const auto& product : shopping_list) {
        // Если товар не у продавца 1, то он у 2 т.к. ввод был обработан
        if (products_seller1.find(product) != products_seller1.end()) {
            seller_number = "1";
        } else {
            seller_number = "2";
        }
        print("покупатель " + customer_number + " делает запрос на покупку товара '"
                             + product + "' продавцу " + seller_number + "\n");
        if (seller_number == "1") {
            gotten_product = ask_seller1(product, customer_number);
        } else {
            gotten_product = ask_seller2(product, customer_number);
        }
        print("покупатель " + customer_number + " получил товар '"
                             + gotten_product + "' от продавца " + seller_number + "\n");
    }
    print("покупатель " + customer_number + " вышел из рынка\n");
    return nullptr;
}

// Метод для добавления продуктов продавцов
void add_sellers_poducts() {
    // Продукты продавца 1
    products_seller1.insert("яблоки");
    products_seller1.insert("груши");
    products_seller1.insert("апельсины");
    products_seller1.insert("мандарины");
    products_seller1.insert("лимоны");
    // Продукты продавца 2
    products_seller2.insert("помидоры");
    products_seller2.insert("морковки");
    products_seller2.insert("тыквы");
    products_seller2.insert("кабачки");
    products_seller2.insert("брокколи");
}

// Метод для вывода продуктов продавцов
void print_products_menus() {
    // Выводим список прдоуктов продавца 1
    std::cout << "продукты от продавца 1:\n";
    for (const auto& product : products_seller1) {
        std::cout << product << " ";
    }
    // Выводим список прдоуктов продавца 2
    std::cout << "\nпродукты от продавца 2:\n";
    for (const auto& product : products_seller2) {
        std::cout << product << " ";
    }
    std::cout << "\n";
}

// Метод для получения списка продуктов
void get_product_lists() {
    std::string customers[] = {"1", "2", "3", "4"};
    std::string string;
    for (int i = 0; i < 4; ++i) {
        std::cout << "для окончания ввода введите '0'\n";
        std::cout << "введите названия товаров для покупателя " + customers[i] + ":\n";
        while (true) {
            std::cin >> string;
            if (string == "0") {
                break;
            } else if ((products_seller1.find(string) != products_seller1.end()) ||
                       (products_seller2.find(string) != products_seller2.end())) {
                if (i == 0) {
                    shopping_list1.push_back(string);
                } else if (i == 1) {
                    shopping_list2.push_back(string);
                } else if (i == 2) {
                    shopping_list3.push_back(string);
                } else {
                    shopping_list4.push_back(string);
                }
            } else {
                std::cout << "ТАКОГО ТОВАРА НЕТ!!!\n";
            }
        }
    }
}

int main() {
    // Строчка для экономии времени на выводе
    std::ios_base::sync_with_stdio(false);
    // Добавляем продукты продавцов
    add_sellers_poducts();
    // Выводим список продуктов продавцов
    print_products_menus();
    // Получаем списки продуктов покупателей
    get_product_lists();
    // Потоки для покупателей
    pthread_t t1, t2, t3, t4;
    // Объявление mutex-ов
    pthread_mutex_init(&mutex_seller1, nullptr);
    pthread_mutex_init(&mutex_seller2, nullptr);
    pthread_mutex_init(&mutex_console_printing, nullptr);
    pthread_mutex_init(&mutex_starting, nullptr);
    // Назначаем задачи потокам
    pthread_create(&t1, nullptr, &go_through_the_shopping_list, nullptr);
    pthread_create(&t2, nullptr, &go_through_the_shopping_list, nullptr);
    pthread_create(&t3, nullptr, &go_through_the_shopping_list, nullptr);
    pthread_create(&t4, nullptr, &go_through_the_shopping_list, nullptr);
    // join-им потоки
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    pthread_join(t4, nullptr);
    // Удаление mutex-ов
    pthread_mutex_destroy(&mutex_starting);
    pthread_mutex_destroy(&mutex_console_printing);
    pthread_mutex_destroy(&mutex_seller2);
    pthread_mutex_destroy(&mutex_seller1);
    return 0;
}
