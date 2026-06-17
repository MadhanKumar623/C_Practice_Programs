#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKS 64
#define BLOCK_SIZE 128

#define CLEAR_LOWER_BITS(reg,pos,bits) reg &= ~(((1<<bits)-1) << ((pos-bits)+1))
#define CLEAR_HIGHER_BITS(reg,pos,bits) reg &= ~(((1<<bits)-1) << pos)

static uint8_t HEAP_MEMORY[BLOCKS][BLOCK_SIZE];           //Original Heap Memory here  we will store the data
static uint8_t TRACK_ARRAY[BLOCKS] = {0};                 //Not storing any data, just an 0/1 array to store the Heap Memory used/unused Blocks


// Reverses all 32 bits in the input value using two masks.
uint32_t reverse_bits(uint32_t num)
{
    uint32_t ret_val = 0;
    uint32_t mask = 0x00000001;
    uint32_t mask1 = 0x00010000;
    uint32_t shift = 31;
    uint32_t shift_1 = 1;
    for(uint32_t i = 0; i < 32; i++)
    {
        if(i < 16)
        {
             ret_val |= ((num & (mask<<(i%16))) << shift);
             shift = shift - 2;
        }
        else
        {
             ret_val |= ((num & (mask1<<(i%16))) >> shift_1);
             shift_1 = shift_1 + 2;
        }
    }
    return ret_val;
}

// Reverses all 32 bits by shifting out one bit at a time from LSB to MSB.
uint32_t reverse_bits_alternate(uint32_t num)     //Extracting the LSB bits one by one and pushing it to left side as MSB
{
   uint32_t ret_val = 0;

   for(int8_t i = 31; i >=0 ; i--)
   {
       ret_val = ((num&0x00000001) << i) | ret_val;
       num = num >> 1;
   }
   return ret_val;
}

// Converts a hexadecimal string into its numeric 32-bit value.
uint32_t hex_string_to_num(char *str)
{
    uint32_t ret_val = 0;
    uint8_t tmp;
    int len = strlen(str);
    for(int8_t i=0; i<len; i++)
    {
        char c = str[i];                      //Getting the MSB Nibble first
        if(c >= '0' && c <= '9')
        {
            tmp = c - '0';
        }
        else if(c >= 'A' && c <= 'F')
        {
            tmp = (c - 'A') + 10;
        }
        else if(c >= 'a' && c <= 'f')
        {
            tmp = (c - 'a') + 10;
        }
        ret_val = (ret_val << 4)|tmp;        //shifting the bits when new bits comes, this is the universal common approach that can be used anywhere
    }
    return ret_val;
}

// Aligns an address upward to the requested alignment boundary.
uint32_t allign_up(uint32_t addr, uint8_t allignment)
{
    return (addr + (allignment - 1)) & (~(allignment-1));
}

// Checks whether the given memory value is stored in little-endian order.
uint8_t is_little_endian(void *ptr, uint8_t lsb)
{
    uint8_t *ptr1 = (uint8_t*)ptr;
    if(*ptr1 == lsb) return 1;
    else return 0;
}

// Copies bytes between buffers and handles overlapping regions safely.
void memcopy_custom(uint8_t *src, uint8_t *dst, uint8_t size_to_copy)
{
    if(src < dst)     //Copy backward if the Destination address is larger than source address
    {
        for(int i=size_to_copy - 1; i>=0; i--)
          dst[i] = src[i];
    }
    else if(src > dst)      //copy forward if the Source address is larger than Destination address
    {
        for(int i=0; i<size_to_copy; i++)
          dst[i] = src[i];
    }
}

// Allocates one fixed-size block from the custom static heap.
void *my_malloc(void)
{
    for(int i=0;i<BLOCKS; i++)
    {
        if(TRACK_ARRAY[i] == 0)
        {
            TRACK_ARRAY[i] = 1;
            return HEAP_MEMORY[i];
        }
    }
}

// Frees a previously allocated block from the custom static heap.
void free_ptr(void *ptr)
{
    for(int i=0;i<BLOCKS;i++)
    {
        if(HEAP_MEMORY[i] == ptr)
        {
            TRACK_ARRAY[i] = 0;
            return;
        }
    }
}

// Rotates an integer array to the left by k positions.
void array_rotate_left(int arr[], int n, int k)
{
   k = k % n;
   for (int r = 0; r < k; r++)
   {
       int temp = arr[0];
       for (int i = 0; i < n - 1; i++)
            arr[i] = arr[i + 1];
       arr[n - 1] = temp;
   }
}

// Rotates an integer array to the right by k positions.
void array_rotate_right(int arr[], int n, int k)
{
   k = k % n;
   for (int r = 0; r < k; r++)       //For counting the number of Rounds
   {
       int temp = arr[n-1];
       for (int i = n-1; i > 0; i--)
            arr[i] = arr[i - 1];
       arr[0] = temp;
   }
}

// Reverses the contents of an integer array in place.
void array_reverse(int arr[], int len)     //Did without the second array creation
{
    int start = 0, end = len-1;
    while(start < end)
    {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++; end--;
    }
}

// Rotates the bits of a 32-bit value to the right.
uint32_t bit_rotate_right(uint32_t num, int r)
{
    uint32_t ret_val = num;
    r = r%32;             //since this for 32 Bits
    while(r > 0)
    {
        ret_val = ret_val >> 1;
        ret_val = ((num<<31) & 0x80000000) | ret_val;
        num = num >> 1;
        r--;
    }
    return ret_val;

    //Below is the another easy method
    //int bits = 32;
    //r = r % bits;
    //return (num >> r) | (num << (bits - r));
}

// Rotates the bits of a 32-bit value to the left.
uint32_t bit_rotate_left(uint32_t num, int r)
{
    uint32_t ret_val = num;
    r = r%32;           //since this for 32 Bits
    while(r > 0)
    {
        ret_val = ret_val << 1;
        ret_val = ((num>>31) & 0x00000001) | ret_val;
        num = num << 1;
        r--;
    }
    return ret_val;

    //Below is the another easy method
    //int bits = 32;
    //r = r % bits;
    //return (num << r) | (num >> (bits - r));
}

// Prints duplicate values in an array using a small seen map.
void print_duplicates(int *arr, int n)
{
    int seen[100] = {0};         //Usage of Hash Map
    for (int i = 0; i < n; i++)
    {
       if (seen[arr[i]])
          printf("Duplicate: %d at Position: %d\n", arr[i],i);
       else
          seen[arr[i]] = 1;
    }
}

// Finds the start and end indices of the maximum-sum subarray.
void getMaxSumSubArrayIndices(int arr[], int size, int *startIndex, int *endIndex)
{
    int currSum = arr[0];
    int maxSum = arr[0];

    int tempStart = 0;

    *startIndex = 0;
    *endIndex = 0;

    for (int i = 1; i < size; i++)
    {
        if (currSum + arr[i] < arr[i])
        {
            currSum = arr[i];
            tempStart = i;
        }
        else
        {
            currSum += arr[i];
        }
        if (currSum > maxSum)
        {
            maxSum = currSum;
            *startIndex = tempStart;
            *endIndex = i;
        }
    }
}

// Swaps the byte order of a 32-bit integer.
uint32_t endianess_conversion_32Bit(uint32_t val)
{
     //0x12345678 --> 0001 0010 0011 0100 0101 0110 0111 1000 ---> 87563412 ---> 0111 1000 0101 0110 0011 0100 0001 0010
    return ((val >> 24)&0x000000FF) | ((val >> 8)&0x0000FF00) | ((val << 8)&0x00FF0000) | ((val << 24)&0xFF000000);
}

// Swaps odd and even bit positions in a 32-bit value.
uint32_t swap_odd_even_bits_32Bit(uint32_t data)
{
    return (((0xAAAAAAAA & data) >> 1) | ((0x55555555 & data) << 1));
}

// Sorts an array containing only 0s and 1s by moving all 0s to the front.
void sortArray_1s0s(uint8_t *arr, uint8_t len)
{
    int left = 0;
    for(uint8_t i=0; i<len; i++)
    {
        if(arr[i] == 0)
        {
            uint8_t temp = arr[i];
            arr[i] = arr[left];
            arr[left] = temp;
            left++;
        }
    }
}

// Removes all occurrences of a character from a string in place.
void deleteCharFromString(char *str, char character, uint8_t len)
{
    /*uint8_t index;
    for(index=0;index<len;index++)
    {
        if(str[index] == character)
        {
            uint8_t locIndex = index;
            for(uint8_t j = index+1; j<len ;j++)
            {
                str[locIndex++] = str[j];
            }
            len = locIndex;
        }
    }
    str[index] = '\0';*/

    //Another approach - Best Approach to avoid the O(n2) in the above method
    uint8_t write_idx = 0;
    for (uint8_t read_idx = 0; read_idx < len; read_idx++)
    {
        if (str[read_idx] != character)
        {
          str[write_idx++] = str[read_idx];
        }
    }
    str[write_idx] = '\0';
}

// Removes all occurrences of a number from an array and updates its size.
void deleteNumFromArray(uint16_t arr[], uint16_t num, uint8_t *size)
{
    uint8_t write_idx = 0;
    uint8_t locSize = *size;
    for(uint8_t read_idx = 0; read_idx < locSize; read_idx++)
    {
        if(arr[read_idx] != num)
        {
            arr[write_idx++] = arr[read_idx];
        }
        else
        {
            *size = *size - 1;
        }
    }
}

// Removes duplicate characters from a string while keeping the first occurrence.
void remove_duplicates(char *str) {
	int index = 0;
    int final_index = 0;
    int map[256] = {0};

    while(str[index])
    {
        if(!map[str[index]])
        {
            str[final_index++] = str[index];
            map[str[index]] = 1;
        }
        index++;
    }
    str[final_index]='\0';
}

//Custom atoi() function implementation
int custom_atoi(char *str)
{
    int ret_val = 0;
    int sign = 1;
    if(str[0] == '-' || str[0] == '+')
    {
        if(str[0] == '-') sign = -1;
        str++;
    }
    while((*str >= '0') && (*str <= '9'))
    {
        ret_val = (ret_val * 10) + (*str - '0');
        str++;
    }
    return ret_val*sign;
}

//Custom string Classifier
void classify_string(const char *str)
{
    char has_num = 0, has_alpha = 0, has_others = 0;
    while(*str != '\0')
    {
        if(((*str >= 'a') && (*str <= 'z')) || ((*str >= 'A') && (*str <= 'Z')))
        {
            has_alpha = 1;
        }
        else if((*str >= '0') && (*str <= '9'))
        {
            has_num = 1;
        }
        else   //Any other characters apart from num and alpha
        {
            has_others = 1;
        }
        str++;
    }
    if(has_num && !has_alpha && !has_others) printf("NUMERIC");
    else if(!has_num && has_alpha && !has_others) printf("ALPHABETIC");
    else if((has_num && has_alpha) || has_others) printf("MIXED");
}

//Custom string to Float conversion function
float custom_atof(const char *str) {
    float ret_val;
    char signFlag = 1;
    char dotFlag = 0;
    long decCount = 1;

    if(str[0] == '+')
    {
        signFlag = 1;
        str++;
    }
    else if(str[0] == '-')
    {
        signFlag = -1;
        str++;
    }

    while(*str != '\0')
    {
        if(*str == '.')
        {
            dotFlag = 1;
            str++;
            continue;
        }
        ret_val = (ret_val*10) + (*str - '0');
        if(dotFlag == 1) decCount = decCount * 10;
        str++;
    }
    ret_val = ret_val * (float)(1.00/decCount);

    return ret_val*signFlag;
}

// Search the key element in an array and send it's position in the array
int binary_search(uint8_t *arr, uint8_t n, uint8_t key)
{
    uint8_t left = 0, right = n-1, mid;

    while(left <= right)
    {
        mid = left + (right - left)/2;     //standard formula for computing the mid element in an array

        if(arr[mid] == key)
        {
            return mid;
        }
        else if(arr[mid] > key)
        {
            right = mid - 1;
        }
        else if(arr[mid] < key)
        {
            left = mid + 1;
        }
    }
    return -1;
}


// Bubble sort algorithm in Ascending order
void bubble_sort(uint8_t *arr, uint8_t n) {
    int global_counter = 0;
    while(global_counter < n)
    {
        for(int j=0;j<n-global_counter-1;j++)
        {
            if(arr[j] > arr[j+1])            //Change this to arr[j] < arr[j+1] for Descending order Bubble sort
            {
                uint8_t temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
        global_counter++;
    }
}


//Function to find the first 3 maximum elements in an array without using of any Sorting logic
void find_top_3(uint8_t *arr, uint8_t n) {
    uint8_t largest1 = arr[0];
    uint8_t largest2 = arr[0];
    uint8_t largest3 = arr[0];

    for(uint8_t i=1; i<n; i++)
    {
        if(arr[i] > largest3 && arr[i] > largest2 && arr[i] > largest1)
        {
            largest3 = largest2;
            largest2 = largest1;
            largest1 = arr[i];
        }
        else if(arr[i] > largest3 && arr[i] > largest2 && arr[i] < largest1)
        {
            largest3 = largest2;
            largest2 = arr[i];
        }
        else if(arr[i] > largest3 && arr[i] <largest2 && arr[i] < largest1)
        {
            largest3 = arr[i];
        }
    }
    if(n == 1) printf("%d\n",largest1);
    else if(n == 2) printf("%d %d\n",largest1, largest2);
    else printf("%d %d %d\n",largest1, largest2, largest3);
}

// Demonstrates one of the utility functions with a sample string.
int main()
{
    char str[20] = "Hello Madhan";
    remove_duplicates(str);
    printf("%s",str);
}
