`ifdef MODEL_TECH
	`include "../sys_defs.vh"
`endif


// `timescale 100ns / 1ps

module rf_processor_tb;

////
//Data Memory AHB
////
// // Parameters
//   localparam MEM_DEPTH = 32768;
//   localparam MEM_32BIT_LINES = (MEM_DEPTH/4);

  // Signals
  logic           HCLK;
  logic           HRESETn;
  logic [31:0]    HADDR;
  logic [2:0]     HSIZE;
  logic           HWRITE;
  logic [31:0]    HWDATA;
  logic           HREADY;
  logic [31:0]    HRDATA;
  logic           HREADYOUT;
  logic           HRESP;
  logic [1:0]     HTRANS;
    

logic [31:0] 	instruction;
logic [31:0] 	pc_addr;
logic [1:0]   im_command;


rf_processor proc_module(
                      .HCLK(HCLK),
                      .HRESETn(HRESETn),
                      .instruction(instruction),
                      .pc_addr(pc_addr),
                      .im_command(im_command),

                      //AHB connnections
                      .HRDATA(HRDATA),     
                      .HREADY(HREADYOUT),  	 
                      .HRESP(HRESP),      
                      .HADDR(HADDR),
                      .HSIZE(HSIZE),
                      .HWRITE(HWRITE),
                      .HWDATA(HWDATA),   
                      .HTRANS(HTRANS)  
);

logic [3:0] mem2proc_response_im;
logic [3:0] mem2proc_tag_im;

logic [31:0] im_data;
assign im_data=0;

mem IM(
       .clk(HCLK),
       .proc2mem_addr(pc_addr),
       .proc2mem_data(im_data),
       .proc2mem_command(im_command),
       .mem2proc_response(mem2proc_response_im),
       .mem2proc_data(instruction),
       .mem2proc_tag(mem2proc_tag_im));


  
  // assign HREADY = 1;//no other subordinates
  assign HREADY = HREADYOUT;

  // Instantiate the DataMemory
  mem_late8 DM (
    .HCLK(HCLK),
    .HRESETn(HRESETn),
    .HADDR(HADDR),
    .HSIZE(HSIZE),
    .HWRITE(HWRITE),
    .HWDATA(HWDATA),
    .HREADY(HREADY),
    .HTRANS(HTRANS),
    .HRDATA(HRDATA),
    .HREADYOUT(HREADYOUT),
    .HRESP(HRESP)
  );

initial begin
	HCLK=0;
	forever #5 HCLK=~HCLK;
end

initial begin
    $readmemh("bb_sort2H.txt",IM.unified_memory);
    // $readmemh("bit_count.txt",IM.unified_memory);
    // $readmemh("gcd_test.txt",IM.unified_memory);
    // $readmemh("mul_test.txt",IM.unified_memory);
    // $readmemh("scalar_demo2.txt",IM.unified_memory);
    // $readmemh("fpga_square1.txt",IM.unified_memory);


    // $readmemh("mem_hex.txt",DM.mem);
    // $readmemh("init_file.txt",DM.mem);
end

initial begin
    HRESETn=0;
    @(posedge HCLK);
    HRESETn=1;
    for(int i=0;i<50000;i++) begin
        @(posedge HCLK);    
    end
    $stop;
end


endmodule