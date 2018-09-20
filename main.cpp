#include "mbed.h"
#include "HCSR04.h"
#include "Servo.h"

DigitalIn PIR(D4);
DigitalOut LED(D10);
AnalogIn LDR(A0);
AnalogIn Gas(A1);
InterruptIn button1(D8);
InterruptIn button2(D6);


Serial pc(USBTX,USBRX);
Serial esp_serial(PA_13,PA_14);

int val,ldr_val,gas_val;
int dist;

HCSR04 ultrasonic(D9,D7); //trigger,echo
Servo servo(D14);


void open()
{   
pc.printf("open");
    servo.position(180);
    }
    

void close()
{   
pc.printf("close");
    servo.position(0);
    }    

int main()
{
    pc.baud(9600);
    esp_serial.baud(115200);
    pc.printf("begin");
    
    button1.rise(&open);
    button2.rise(&close);
    
    while(1) {
        
        val = PIR.read();
        if(val==1) {
            
            pc.printf("Motion Detected\n");
            dist = ultrasonic.distance(1);
            pc.printf("the distance is %d cm\n",dist);
            esp_serial.printf("U");
            esp_serial.printf("%d",dist);
            esp_serial.putc('\0');

            if (dist>10.0) {
                servo.position(180);
                wait(5);
                servo.position(0);
            }

            } else {
                pc.printf("Motion ended\n");
                
            }
            
            
            
        ldr_val = LDR.read_u16();
            
        pc.printf("ldr: %d\n",ldr_val);
        esp_serial.printf("L");
        esp_serial.printf("%d",ldr_val);
        esp_serial.putc('\0');
        if(ldr_val <=300)
        LED = 1;
        
        else 
        LED = 0;
        wait(2);
        
        
        gas_val = Gas.read_u16();
        pc.printf("gas: %d\n",gas_val);
        esp_serial.printf("G");
        esp_serial.printf("%d",gas_val);
        esp_serial.putc('\0');
        //wait(10);
        }
    }
    
    
