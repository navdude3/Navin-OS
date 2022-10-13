#define handle(handlername, handler_id) 

    .global handlername        
    handlername:                           
       # pushal                                
       # pushfl   
        pushl $handler_id              
        call handlername
        add $4, %esp         
       # popfl                  
       # popal                  
        iret 



handle(divide_error, 0);
handle(RESERVED_1, 1);
handle(nmi_int, 2);
handle(breakpoint, 3);
handle(overflow, 4);
handle(bound_range, 5);
handle(invalid_opcode, 6);
handle(device_not_available, 7);
handle(double_fault, 8);
handle(segment_overrun, 9);
handle(invalid_tss, 10);
handle(seg_not_present, 11);
handle(seg_fault, 12);
handle(general_protection, 13);
handle(page_fault, 14);
handle(RESERVED_15, 15);
handle(floating_point_error, 16);
handle(alignment_check, 17);
handle(machine_check, 18);
handle(simd_floating_point_exception, 19);