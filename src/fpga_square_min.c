#pragma GCC push_options
#pragma GCC optimize("O0")
int multiply(int a, int b) {
    int result = 0;
    int negative = 0;
    int temp;
    if (b < 0) {
        b = -b;
        negative = 1;
    }
    while (b > 0) {
        if (b & 1) result += a;
        a <<= 1;
        b >>= 1;
    }
    __asm__ volatile("mv %0, %1" :"=r"(temp):"r"(b));
    return negative ? -result : result;
}
#pragma GCC pop_options

void set_base(int *x, int *y, int *x_b, int *y_b) {
    int i;
    int val;

    int *x_ptr = x;
    int *y_ptr = y;
    int *x_src = x_b;
    int *y_src = y_b;

    for (i = 0; i < 20; i++) {
        // Copy x
        __asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(x_src));
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr));
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_src));
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));

        // Copy y
        __asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(y_src));
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_src));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    }
}

void push_frame(int *x, int *y, int avl){
    int* x_ptr = x;
    int* y_ptr = y;
    int* fb_ptr = (int*)65536;
    int i;
    int center = 64;
    int column_width = 128;

    for (i = 0; i < avl; i++) {
        int x_val, y_val;
        int screen_x, screen_y;
        int pixel_index;

        // Load x[i] and y[i]
        __asm__ volatile("lw %0, 0(%1)" : "=r"(x_val) : "r"(x_ptr));
        __asm__ volatile("lw %0, 0(%1)" : "=r"(y_val) : "r"(y_ptr));

        // Map to screen space
        screen_x = x_val + center;
        screen_y = center - y_val;

        // Use your multiply function for screen_y * column_width
        int row_offset = multiply(screen_y, column_width);

        pixel_index = screen_x + row_offset;

        // Store pixel index to framebuffer
        __asm__ volatile("sw %0, 0(%1)" :: "r"(pixel_index), "r"(fb_ptr));

        // Advance all pointers
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
        __asm__ volatile("addi %0, %0, 4" : "+r"(fb_ptr));
    }

}


void pulse_animation(
    int* x_base, int* y_base,
    int scale,
    int avl
) {
    int* x_b = x_base;
    int* y_b = y_base;
    int* fb_ptr = (int*)65536;

    int center = 64;
    int column_width = 128;
    int cx = 0;
    int cy = 0;

    for (int i = 0; i < avl; i++) {
        int x0, y0, dx, dy, x_val, y_val, screen_x, screen_y, pixel_index, row_offset;

        // Load base coordinates
        __asm__ volatile("lw %0, 0(%1)" : "=r"(x0) : "r"(x_b));
        __asm__ volatile("lw %0, 0(%1)" : "=r"(y0) : "r"(y_b));
        // x0 = *x_b;
        // y0 = *y_b;

        // Compute dx = x0 - cx
        dx = x0 - cx;
        dy = y0 - cy;

        // Scale around center
        dx = multiply(dx, scale);
        dy = multiply(dy, scale);

        x_val = cx + dx;
        y_val = cy + dy;

        // Map to screen
        screen_x = x_val + center;
        screen_y = center - y_val;
        row_offset = multiply(screen_y, column_width);
        pixel_index = screen_x + row_offset;

        // Write to framebuffer
        __asm__ volatile("sw %0, 0(%1)" :: "r"(pixel_index), "r"(fb_ptr));

        // Move pointers
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_b));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_b));
        __asm__ volatile("addi %0, %0, 4" : "+r"(fb_ptr));
    }
}

int o;
int delay(int cycles)
{
	int k=0;
	int i;
	for(i=0;i<cycles;i++) //1000000
	{
		k=k+i;
	}
	return k;
}

void main(){
    int i;
    int val;
    int x_square[20];
    int y_square[20];

    int x_pos[20];
    int y_pos[20];

    int x_rand[20];
    int y_rand[20];

    //square initialization
    int *x_s = x_square;
    int *y_s = y_square;
    //upper barrier
    for(i=0; i<5; i++){
        // x_square[i]=-58+(i*5);
        val = -12 + multiply(5, i);
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_s));// apo 4 se 1
    }
    for(i=0; i<5; i++){
        // y_square[i]=-58;
        val = -12-8;
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_s));
    }

    //rs
    for(i=5; i<10; i++){
        // x_square[i]=-34;
        val = 12+8;
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_s));// apo 4 se 1
    }
    for(i=5; i<10; i++){
        // y_square[i]=-58+((i-5)*5);
        val = -12+multiply(i-5,5);
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_s));
    }

    //lower barrier
    for(i=10; i<15; i++){
        // x_square[i]=-34-((i-10)*5);
        val = 12-multiply(i-10,5);
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_s));// apo 4 se 1
    }
    for(i=10; i<15; i++){
        // y_square[i]=-34;
        val = 12+8;
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_s));
    }

    //ls
    for(i=15; i<20; i++){
        // x_square[i]=-58;
        val = -12-8;
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(x_s));// apo 4 se 1
    }
    for(i=15; i<20; i++){
        // y_square[i]=-34-((i-15)*5);
        val = 12-multiply(i-15,5);
        __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_s));
        __asm__ volatile("addi %0, %0, 4" : "+r"(y_s));
    }

    //rand initialization
    int* x_ptr = x_rand;


    // x_rand initialization
    val = -18; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -14; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -11; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 0; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 9; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 24; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 30; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr));
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 34; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 19; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 27; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 5; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -8; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -23; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -31; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -34; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 13; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -28; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 6; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = 16; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr));
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));
    val = -3; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_ptr));
    __asm__ volatile("addi %0, %0, 4" : "+r"(x_ptr));

    int* y_ptr = y_rand;

    // y_rand initialization
    val = -42; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -27; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -34; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -31; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -38; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -20; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -13; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -2; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 3; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 14; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 26; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 23; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 8; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 17; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = 11; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -7; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -10; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -14; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -16; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));
    val = -24; __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_ptr)); 
    __asm__ volatile("addi %0, %0, 4" : "+r"(y_ptr));


    //animation loop
    int currentPoint = 0;
    int x_pos_done =0;
    int y_pos_done =0;
    while (1)
    {
        push_frame(&x_rand, &y_rand, 20);
        o=delay(1000000);

        set_base(&x_pos, &y_pos, &x_rand, &y_rand);

        //move to sides
        
        currentPoint = 0;
        x_pos_done = 0;
        y_pos_done = 0;

        while(currentPoint<20){

        int shift, x_offset, y_offset;
        int x_val, x_target, y_val, y_target;

        int *x_p = x_pos;
        int *y_p = y_pos;

        int *x_s = x_square;
        int *y_s = y_square;

        // Shift currentPoint index by 2 (<<2)
        __asm__ volatile("slli %0, %1, 1" : "=r"(shift) : "r"(currentPoint));
        __asm__ volatile("slli %0, %0, 1" : "+r"(shift));

        // Calculate offsets
        __asm__ volatile("add %0, %1, %2" : "=r"(x_offset) : "r"(x_p), "r"(shift));
        __asm__ volatile("add %0, %1, %2" : "=r"(y_offset) : "r"(y_p), "r"(shift));

        // Load current x and y
        __asm__ volatile("lw %0, 0(%1)" : "=r"(x_val) : "r"(x_offset));
        __asm__ volatile("lw %0, 0(%1)" : "=r"(y_val) : "r"(y_offset));

        // Load target x and y
        __asm__ volatile("add %0, %1, %2" : "=r"(x_offset) : "r"(x_s), "r"(shift));
        __asm__ volatile("add %0, %1, %2" : "=r"(y_offset) : "r"(y_s), "r"(shift));
        __asm__ volatile("lw %0, 0(%1)" : "=r"(x_target) : "r"(x_offset));
        __asm__ volatile("lw %0, 0(%1)" : "=r"(y_target) : "r"(y_offset));

        // Compare and update x
        if (x_val < x_target) x_val++;
        else if (x_val > x_target) x_val--;
        else x_pos_done = 1;

        // Compare and update y
        if (y_val < y_target) y_val++;
        else if (y_val > y_target) y_val--;
        else y_pos_done = 1;

        // Store updated values
        __asm__ volatile("add %0, %1, %2" : "=r"(x_offset) : "r"(x_p), "r"(shift));
        __asm__ volatile("add %0, %1, %2" : "=r"(y_offset) : "r"(y_p), "r"(shift));
        __asm__ volatile("sw %0, 0(%1)" :: "r"(x_val), "r"(x_offset));
        __asm__ volatile("sw %0, 0(%1)" :: "r"(y_val), "r"(y_offset));

        if((x_pos_done==1) && (y_pos_done==1)){
            currentPoint++;
            x_pos_done=0;
            y_pos_done=0;
        }

        push_frame(&x_pos, &y_pos, 20);
        o=delay(100000);
    }

    
    //moves sides to final position
    int step_sides = 1;
    int move_initial = 8;
    int temp=0;
    while(temp<move_initial){
        int *y_p = y_pos;
        for(i=0; i<5; i++){
            // y_pos[i]-=1;
            __asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(y_p));
            val += step_sides;
            __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_p));
            __asm__ volatile("addi %0, %0, 4" : "+r"(y_p));
        }
        push_frame(&x_pos, &y_pos, 20);
        o=delay(500000);
        temp++;
    }

    //move rs
    temp = 0;
    while(temp<move_initial){
        int *x_p = x_pos;
        __asm__ volatile("addi %0, %0, 20" : "+r"(x_p));  // x + 5 * 4
        for(i=5; i<10; i++){
            // x_pos[i]+=1;
            __asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(x_p));
            val -= step_sides;
            __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_p));
            __asm__ volatile("addi %0, %0, 4" : "+r"(x_p));
        }
        temp++;
        push_frame(&x_pos, &y_pos, 20);
        o=delay(500000);
    }
    //move bottom
    temp = 0;
    while(temp<move_initial){
        int *y_p = y_pos;
        __asm__ volatile("addi %0, %0, 40" : "+r"(y_p)); 
        for(i=10; i<15; i++){
            // y_pos[i]+=1;
            __asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(y_p));
            val -= step_sides;
            __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(y_p));
            __asm__ volatile("addi %0, %0, 4" : "+r"(y_p));
        }
        temp++;
        push_frame(&x_pos, &y_pos, 20);
        o=delay(500000);
    }
    //move ls
    temp=0;
    while(temp<move_initial){
        int *x_p = x_pos;
        __asm__ volatile("addi %0, %0, 60" : "+r"(x_p));  // x + 15 * 4
        for(i=15; i<20; i++){
            // x_pos[i]-=1;
            __asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(x_p));
            val += step_sides;
            __asm__ volatile("sw %0, 0(%1)" :: "r"(val), "r"(x_p));
            __asm__ volatile("addi %0, %0, 4" : "+r"(x_p));
        }
        temp++;
        push_frame(&x_pos, &y_pos, 20);
        o=delay(500000);
    }

    o=delay(1000000);

    //pulse animation
    pulse_animation(&x_pos, &y_pos, 2, 20);
    o=delay(1000000);

    pulse_animation(&x_pos, &y_pos, 3, 20);
    o=delay(1000000);

    pulse_animation(&x_pos, &y_pos, 2, 20);
    o=delay(1000000);

    push_frame(&x_pos, &y_pos, 20);
    o=delay(1000000);
   }
    
}