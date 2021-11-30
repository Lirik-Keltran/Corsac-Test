#ifdef TEST_ENABLE

#define Testing [=]()mutable

#endif

#ifdef TEST_BLOCK

#define TestingBlock [=](corsac::Block* assert)mutable

#endif

#ifndef CORSAC_TEST
#define CORSAC_TEST

#define STANDART 6
#define GREEN 10
#define RED 12
#define YELLOW 14

#include <functional>
#include <vector>
#include <windows.h> 
#include <string>
#include <iostream>
#include <ctime>
#include <typeinfo>



namespace corsac
{
	namespace internel
	{
		/*
			@desc - Ставит нужное кол-во пробелов.

			@param amount - Кол-во пробелов.
		*/		
		void space_print(int amount)
		{
			for (int i = 0; i <= amount; i++)
			{
				std::cout << " ";
			}
		}


		/*
			@desc - Устанавливает цвет текста.

			@param color - Номер цвета.
		*/		
		void set_color(int color)
		{

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(hConsole, color);
		}

		/*
			@desc - Печатает текст в консоли заданого цвета.
		*/
		void print(std::string text, int color)
		{
			internel::set_color(color);
			std::cout << text;
		}

		/*
			@desc - Печатает комментарий.
		*/
		void print_comment(std::string text, int color, int amount_space)
		{
			if(text.size() > 0)
			{
				internel::space_print(amount_space);
				internel::print("> "+text+"\n", color);
			}
		}
	}

	/*
		@desc - Структура теста.

		@field test 			- Лямбда функция с тестом.
		@field name 			- Имя теста.
		@field comment 			- Комментарий к тесту.
		@field comment_color 	- Цвет комментария.
	*/		
	class Test
	{
		private:
			std::function<bool()> test;
			std::string name;
			std::string comment;
			int comment_color;
			bool result;

		public:
			Test(std::string name, std::function<bool()> func)
			{
				this->test = func;
				this->name = name;
			}

			void set_comment(std::string comment, int color)
			{
				this->comment = comment;
				this->comment_color = color;
			}

			void nt()
			{
				auto condition = this->test;
				this->test = [=]()mutable{ return !condition();};
			}

			bool start_test()
			{
				this->result = this->test();
				return this->result;
			}

			void print_result(int amount_space)
			{
				internel::space_print(amount_space);

				if(this->result)
				{
					internel::print("\xfb:    " + this->name + "\n", GREEN);
				}
				else
				{
					internel::print("X:    " + this->name + "\n", RED);
				}

				internel::print_comment(this->comment, 
										this->comment_color,
										amount_space+1);
			}

			bool check_result()
			{
				return this->result;
			}
	};

	class Block
	{
		private:
			std::string name_block;
			std::string block_comment;

			std::vector<Test> tests;
			std::vector<Block> blocks;


			int amount_space = 0;
			int comment_color = STANDART;

			/* 
				@desc - Добавляет новый блок тестов.

				@param name - Имя подблока.
			*/		
			Block* create_block(std::string name)
			{
				this->blocks.push_back(Block(name, this->amount_space+3));

				return &this->blocks[this->blocks.size()-1];
			}

			/*
				@desc - Пишет имя блока.
			*/
			void print_block_name()
			{
				internel::space_print(this->amount_space);
				internel::print(this->name_block + ": \n", STANDART);
				internel::print_comment(this->block_comment, 
										this->comment_color,
										this->amount_space+1);
			}			

			/*
				@desc - Запускает тесты и подтесты.
			*/
			int start_tests()
			{
				unsigned int start_time = clock();
				int error = 0;

				for(int i = 0; i < this->tests.size(); i++)
				{
					if(!this->tests[i].start_test())
					{
						error++;
					}
				}

				this->print_results(start_time, error);

				for(auto i = 0; i < this->blocks.size(); i++)
				{
					error += this->blocks[i].start_tests();
				}

				return error;
			}

			void print_results(unsigned int time, int error)
			{
				#ifndef CORSAC_TEST_RESULT_OFF

				#ifdef CORSAC_TEST_WITHOUT_OK
					if(error != 0)
					{
				#endif

				this->print_block_name();

				for(int i = 0; i < this->tests.size(); i++)
					#ifdef CORSAC_TEST_WITHOUT_OK
						if(!this->tests[i].check_result())
					#endif
					this->tests[i].print_result(this->amount_space);


				unsigned int end_time = clock(); 
				unsigned int search_time = end_time - time; 
				internel::set_color(YELLOW);

				internel::space_print(this->amount_space);
				internel::print("Test time: "+std::to_string(search_time)+"ms\n", YELLOW);
				#ifdef CORSAC_TEST_WITHOUT_OK
					}
				#endif

				#endif
			}


		public:
			/*
				@param name 	- Имя блока теста.
				@amount_space 	- Задает кол-во отступов.
			*/
			Block(std::string name, int amount_space)
			{
				this->name_block = name;
				this->amount_space = amount_space;
			};


			/*
				@param name - Имя блока теста.
			*/
			Block(std::string name)
			{
				this->name_block = name;
				this->amount_space = amount_space;
			};

			/*
				@desc - Возвращает кол-во тестов.
			*/		
			int amount_test(){
				int sum = this->tests.size();
				for (int i = 0; i < this->blocks.size(); ++i)
				{
					sum += this->blocks[i].amount_test(); 
				}

				return sum;
			}

			/*
				@desc - Устонавливает базовый цвет текста комментария.

				@param color - Номер цвета в консоли.
			*/		
			void set_comment_color(int color)
			{
				this->comment_color = color;
			}


			/*
				@desc - Добавляет под блок для тестирования.

				@note - Может использовать макрос TestBlock для сокращения записи, 
						для этого надо прописать #define TEST_BLOCK.
				@exemple - assert.add_block("Block1", TestBlock{ 
								assert.equal("Equal", 1, 1);
							});


				@param name - Имя теста.
				@param test - Лямбда функция с набором тестом.
			*/		
			void add_block(std::string name, std::function<void(Block*)> func_block)
			{
				func_block(this->create_block(name));		
			}


			/*
				@desc - Добавляет кастомый тест.

				@note - функция должна возвращать булево значение.
				@note - Может использовать макрос Test для сокращения записи, 
						для этого надо прописать #define TEST_ENABLE.
				@exemple - assert.add("custom test", Test{ 
								...code
							});


				@param name - Имя теста.
				@param test - Лямбда функция с тестом, возвращает булево.
			*/
			Block* add(std::string name, std::function<bool()> test)
			{
				this->tests.push_back(Test(name, test));
			
				return this;
			}

			/*
				@desc - Сравнивает ссылку двух аргументов.

				@param name 	- Имя теста.
				@param f 		- Ссылка на первый аргумент.
				@param t 		- Ссылка на второй аргумент.
			*/
			template <typename T, typename C>
			Block* equal_ref(std::string name, T* f, C* t){
				this->add(name, [=]()mutable
				{
					if(f == t)
						return true;

					return false;
				});

				return this;
			}

			/*
				@desc - Сравнивает два аргумента.

				@note - Тип аргументов должен иметь перегрузку, чтобы их можно было сравнить

				@param name 	- Имя теста.
				@param f 		- Первый сравниваемый аргумент.
				@param t 		- Второй сравниваемый аргумент.
			*/	
			template <typename T, typename C>
			Block* equal(std::string name, T f, C t){
				this->add(name, [=]()mutable
				{
					if(f == t)
						return true;

					return false;
				});

				return this;
			}

			/*
				@desc - Инвертирует ожидаемый результат теста.
			*/	
			Block* nt()
			{
				this->tests.back().nt();
			
				return this;
			}

			/*
				@desc - Проверяет на ложь.

				@param name 	- Имя теста.
				@param f 		- Проверяемый аргумент.
			*/	
			template <typename T>
			Block* is_false(std::string name, T f)
			{
				this->add(name, [=]()mutable
				{
					return f == false;
				});
				return this;
			}

			/*
				@desc - Проверяет на правду.

				@param name 	- Имя теста.
				@param f 		- Проверяемый аргумент.
			*/		
			template <typename T>
			Block* is_true(std::string name, T f)
			{
				this->add(name, [=]()mutable
				{
					return f == true;
				});
				return this;
			}

			/*
				@desc - Сравнивает типы элементов.

				@param name 	- Имя теста.
				@param f 		- Первый аргумент.
				@param t 		- Второй аргумент с которым надо сравнить.
			*/
			template <typename T, typename C>
			Block* is(std::string name, T f, C t)
			{
				this->add(name, [=]()mutable
				{
					if(typeid(f).name() == typeid(t).name())
						return true;

					return false;
				});

				return this;
			}


			/*
				@desc - Проверяет контеинер на наличие в нем переданного элемента.

				@param name		- Имя теста.
				@param start 	- Итератор начала массива.
				@param end 		- Итератор конца массива.
				@param search 	- Элемент который ищется в массиве.  
			*/
			template <typename iter, typename T>
			Block* is_contein(std::string name, iter start, iter end, T search)
			{
				this->add(name, [=]()mutable
				{
					bool find = false;

					for(; start != end; ++start)
					{
						if(*start == search){
							find = true;
							break;
						}
					}
	            	return find;
				});	
				return this;
			}

			/*
				@desc - Добавляет комментарий после блока.

				@param comment - Текст комментария.  
			*/
			void add_block_comment(std::string comment)
			{
				this->block_comment = comment;
			}

			/*
				@desc - Добавляет комментарий после теста.

				@param comment - Текст комментария.  
			*/
			void add_comment(std::string comment)
			{
				this->tests.back().set_comment(comment, this->comment_color);
			}

			/*
				@desc - Добавляет комментарий после теста.

				@param comment 	- Имя теста.
				@param color 	- Получает номер цвета текста.  
			*/
			void add_comment(std::string comment, int color)
			{
				this->tests.back().set_comment(comment, color);
			}


			/*
				@desc - Запускает тесты и подтесты.
			*/		
			void start()
			{
				int error = this->start_tests();
				internel::set_color(GREEN);
				std::cout << " \xfb:    " << this->amount_test()-error << std::endl;
				
				internel::set_color(RED);
				std::cout << " X:    " << error << std::endl;

				internel::set_color(STANDART);
			}
			
};
}
#endif