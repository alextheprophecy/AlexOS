.section .text_data

.global __res_data_start
__res_data_start:
    .incbin "res/data.txt"
    
.global __res_data_end
__res_data_end:

.global __res_data_size
__res_data_size:
    .int __res_data_end - __res_data_start
    .byte 0.section .text_data

.global __res_data2_start
__res_data2_start:
    .incbin "res/data2.txt"
    
.global __res_data2_end
__res_data2_end:

.global __res_data2_size
__res_data2_size:
    .int __res_data2_end - __res_data2_start
    .byte 0