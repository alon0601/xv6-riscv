#include "ustack.h"

void my_assert(int condition, const char* msg) {
    if (!condition) {
        printf("%s\n", msg);
        exit(1);
    }
}

void test_ustack() {
    void* ptr1;
    void* ptr2;
    void* ptr3;
    void* ptr4;
    int result;

    // Test 1: Normal allocation
    ptr1 = ustack_malloc(100);
    my_assert(ptr1 != (void*)-1, "Test 1 failed");

    // Test 2: Allocation of 50 size
    ptr2 = ustack_malloc(50);
    my_assert(ptr2 != (void*)-1, "Test 2 failed");
    
    // Test 3: Exceeding max allocation size
    ptr3 = ustack_malloc(513);
    my_assert(ptr3 == (void*)-1, "Test 3 failed");

    // Test 4: Freeing last allocated buffer
    result = ustack_free();
    printf("res 4: %d\n",result);
    my_assert(result == 50, "Test 4 failed");  // Since last allocated buffer was of size 50

    // Test 5: Freeing when stack is empty
    printf("spointer %d\n",get_ptr());
    result = ustack_free();
    printf("res 5: %d\n",result);
    my_assert(result == 100, "Test 5 failed");
    
    // Test 6: Allocation after freeing
    ptr4 = ustack_malloc(50);
    my_assert(ptr4 != (void*)-1, "Test 6 failed");
    printf("fook ye\n");
    // Test 7: Freeing last allocated buffer after reallocation
    result = ustack_free();
    my_assert(result == 50, "Test 7 failed");  // Since last allocated buffer was of size 50
    printf("res 7: %d\n",result);

    // Test 8: Freeing when stack is empty after reallocation
    result = ustack_free();
    my_assert(result == -1, "Test 8 failed");
    printf("res 8: %d\n",result);

    printf("All tests passed successfully!\n");
}

int main() {
    test_ustack();
    return 0;
}