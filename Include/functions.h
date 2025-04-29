/*
 * April 21, 2025
 * Author: Derrick Tweneboah
 *
 * Description:
 * This file contains the function prototypes used throughout the project.
 */

 #ifndef INCLUDE_FUNCTIONS_H
 #define INCLUDE_FUNCTIONS_H
 
 // Main function
 void main(void);
 
 // Initialization functions
 void Init_Conditions(void);
 
 // State machine
 void Carlson_StateMachine(void);
 
 // Interrupts
 void enable_interrupts(void);
 __interrupt void Timer0_B0_ISR(void);
 __interrupt void switch_interrupt(void);
 
 // Clocks
 void Init_Clocks(void);
 
 // LED Configurations
 void Init_LEDs(void);
 void IR_LED_control(char selection);
 void Backlite_control(char selection);
 
 // LCD Display
 void Display_Process(void);
 void Display_Update(char p_L1, char p_L2, char p_L3, char p_L4);
 void enable_display_update(void);
 void update_string(char *string_data, int string);
 void Init_LCD(void);
 void lcd_clear(void);
 void lcd_putc(char c);
 void lcd_puts(char *s);
 void lcd_power_on(void);
 void lcd_enter_sleep(void);
 void lcd_exit_sleep(void);
 void Write_LCD_Ins(char instruction);
 void Write_LCD_Data(char data);
 void ClrDisplay(void);
 void ClrDisplay_Buffer_0(void);
 void ClrDisplay_Buffer_1(void);
 void ClrDisplay_Buffer_2(void);
 void ClrDisplay_Buffer_3(void);
 void SetPostion(char pos);
 void DisplayOnOff(char data);
 void lcd_BIG_mid(void);
 void lcd_BIG_bot(void);
 void lcd_120(void);
 void lcd_4line(void);
 void lcd_out(char *s, char line, char position);
 void lcd_rotate(char view);
 void lcd_write(unsigned char c);
 void lcd_write_line1(void);
 void lcd_write_line2(void);
 void lcd_write_line3(void);
 void lcd_command(char data);
 void LCD_test(void);
 void LCD_iot_meassage_print(int nema_index);
 
 // SPI (Serial Peripheral Interface)
 void Init_SPI_B1(void);
 void SPI_B1_write(char byte);
 void spi_rs_data(void);
 void spi_rs_command(void);
 void spi_LCD_idle(void);
 void spi_LCD_active(void);
 void SPI_test(void);
 void WriteIns(char instruction);
 void WriteData(char data);

 // Switches
 void Init_Switches(void);
 void switch_control(void);
 void enable_switch_SW1(void);
 void enable_switch_SW2(void);
 void disable_switch_SW1(void);
 void disable_switch_SW2(void);
 void Switches_Process(void);
 void Init_Switch(void);
 void Switch_Process(void);
 void Switch1_Process(void);
 void Switch2_Process(void);
 void menu_act(void);
 void menu_select(void);


 // Menu
 void Menu_Process(void);
 
 // Ports
 void Init_Ports(void);
 void Init_Port1(void);
 void Init_Port2(void);
 void Init_Port3(void);
 void Init_Port4(void);
 void Init_Port5(void);
 void Init_Port6(void);
 
 //Main System
 void initialize_hardware(void);
 void handle_switches(void);
 void handle_received_data(void);
 void handle_commands(void);
 void handle_tasks(void);
 void clear_buffers(void);
 void handle_sending_commands(void);


 // Motor Control
 void Init_motors(void);
 void motors_forward(void);
 void motors_reverse(void);
 void motors_off(void);
 void right_forward(void);
 void left_forward(void);
 void clockwise(void);
 void counterclockwise(void);
 void straight(void);
 void turn(void);
 void stop(void);
 void turn_right(void);
 void turn_left(void);
 void backwards(void);
 void forwards(void);
 void ControlForward(void);
 void Cturn_right(void);
 void Cturn_left(void);
 void rightboard_turn(void);
 void rightboard_turn2(void);
 void leftboard_turn(void);
 
 // Timers
 void enableDisplay(void);
 void Init_timer_B0(void);
 void Init_timer_B1(void);
 void Init_Timer_B3(void);
 
 // ADC (Analog-to-Digital Converter)
 void Init_ADC(void);
 void Init_port1ADC(void);
 void Init_port5ADC(void);
 void Init_DAC(void);
 
 // Speed and Readings
 int cap_speed(int, unsigned int, unsigned int);
 void update_car_speed(unsigned int, unsigned int);
 void HEXtoBCD(int);
 void HEXtoBCD_2(int);
 void HEXtoBCD_3(int);
 void adc_line_1(int);
 void adc_line_2(int);
 void timer_update(unsigned int);
 void clear_display_line(int);
 
 // UART
 void Init_UART_A0(void);
 void Init_UART_A1(void);
 
 // IoT
 void Init_IOT(void);
 void parse_and_execute_commands(char* input);
 void parse_ip_address(char* input);
 void changedIPlines(const char* ip);
 void parse_ssid_address(char* input);
 void center_text(char* dest, const char *src, int width);
 void Transmit_String(void);
 void Update_Display(char *message);
 void send_at_command(const char* command);
 
 // Miscellaneous
 void usleep(unsigned int usec);
 void usleep10(unsigned int usec);
 void five_msec_sleep(unsigned int msec);
 void measure_delay(void);
 void out_control_words(void);
 void followblackline(void);
 void go_autonomous(void);
 void end_autonomous(void);
 
 #endif // INCLUDE_FUNCTIONS_H
