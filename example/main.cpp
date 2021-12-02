#define TEST_BLOCK

#define CORSAC_TEST_WITHOUT_OK
#define CORSAC_TEST_RESULT_OFF
#include "../include/Test.h"

int main()
{
	auto assert = corsac::Block("Global Test Block");

	assert.set_comment_color(YELLOW);

	assert.equal("test - 0", 0, 1);
	assert.equal("test - 1", 0, 1);
	assert.equal("test - 2", 0, 1);
	assert.equal("test - 3", 0, 1);
	assert.equal("test - 4", 0, 1)->nt();
	assert.equal("test - 5", 1, 1)->add_comment("Check Comment");
	assert.equal("test - 6", 1, 1)->add_comment("Check Castom color", STANDART);
	assert.equal("test - 7", 1, 1);

	assert.add_block("Test Block 1", TestingBlock{
		assert->equal("test - 8", 1, 1);
		assert->equal("test - 9", 1, 0);
		assert->equal("test - 10", 1, 0);

		assert->add_block_comment("Check");
	});


	assert.start();

	return 0;
}