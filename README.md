# Corsac-Test
Библиотека для тестирования с++ кода.

## Поддерживая платформа
Win10 
С++ 17 и выше

## Примеры
```c++

//Добавляет возможность простым синтаксисисом создавать блоки.
#define TEST_BLOCK  
#include "./include/Test.h"

//Создаем экземпляр класса.
auto assert = corsac::Block("Global Test Block");

/*
  Устанавливает базовый цвет текста комментария.

  color - Номер цвета в консоли.
*/	
assert.set_comment_color(YELLOW);

/*
  Сравнивает два аргумента.

  Тип аргументов должен иметь перегрузку, чтобы их можно было сравнить

  name 	- Имя теста.
  f 		- Первый сравниваемый аргумент.
  t 		- Второй сравниваемый аргумент.
*/	
assert.equal("test - 0", 0, 1);

/*
  Сравнивает ссылку двух аргументов.

  name 	- Имя теста.
  f 		- Ссылка на первый аргумент.
  t 		- Ссылка на второй аргумент.
*/
assert.equal_ref("test - 1", 0, 1);

assert.equal("test - 2", 0, 1);
assert.equal("test - 3", 0, 1);

/*
  nt() - Инвертирует ожидаемый результат теста.
*/
assert.equal("test - 4", 0, 1)->nt();

/*
  Проверяет аргумент на ложь.

  name 	- Имя теста.
  f 		- Проверяемый аргумент.
*/	
assert.is_false("test - 5", 1, 1)->add_comment("Check Comment");

/*
  Проверяет аргумент на правду.

  name 	- Имя теста.
  f 		- Проверяемый аргумент.
*/
assert.is_true("test - 6", 1, 1)->add_comment("Check Castom color", STANDART);

/*
  Сравнивает типы элементов.

  name 	- Имя теста.
  f 		- Первый аргумент.
  t 		- Второй аргумент с которым надо сравнить.
*/
assert.is("test - 7", 1, 1);

/*
  Проверяет контеинер на наличие в нем переданного элемента.

  name		- Имя теста.
  start 	- Итератор начала массива.
  end 		- Итератор конца массива.
  search 	- Элемент который ищется в массиве.  
*/
assert.is_contein("test - 7", 1, 1);

//Создаем подблок для тестирования
assert.add_block("Test Block 1", TestingBlock{
  assert->equal("test - 8", 1, 1);
  assert->equal("test - 9", 1, 0);
  
  /*
    Добавляет комментарий после теста.

    comment - Текст комментария.  
  */
  assert->equal("test - 10", 1, 0).add_comment("comment");

  
  /*
    Добавляет комментарий после теста.

    comment - Имя теста.
    color 	- Устанавливает номер цвета комментария.  
  */
  assert->equal("test - 10", 1, 0).add_comment("comment", RED);

  /*
    Добавляет комментарий после блока.  

    comment - Текст комментария.  
  */
  assert->add_block_comment("Check");
});
```
