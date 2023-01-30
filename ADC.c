#include "lib/include.h"

/* Habilitamos los 3 canales del módulo 0 del PWM0 para así poder controlar 3 potenciometros
   a una velocidad de conversión de 250kps según la pág 1056.
   Canal 1 -> PE2 - secuenciador 0 
   Canal 2 -> PE1 - secuenciador 1
   Canal 8 -> PE5 - secuenciador 2 
    */

extern void Configura_Reg_ADC0(void)
{
    SYSCTL->RCGCADC = (1<<0);//Habilitamos el módulo 0 de reloj del ADC pág 369
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<0);//Encendemos los puertos necesarios pág 382
    GPIOE_AHB->DIR = (0<<1) | (0<<2) | (0<<5); //Configura i/o PE1, PE2 y PE5 pág 760
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<2) | (1<<5); //Habilitamos la función alternativa PE1, PE2 y PE5 pág 770
    GPIOE_AHB->DEN = (0<<1) | (0<<2) | (0<<5); //Para indicar si es digital o no PE1, PE2 y PE5 pág 781
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FF00F); //Poner 0 en los bits de los pines 1, 2, 5 pág 787 y tabla pág 1808
    GPIOE_AHB->AMSEL = (1<<1) | (1<<2) | (1<<5); //Habilitamos el canal analógico PE1, PE2 y PE5 pág 786

    ADC0->PC = (0<<2)|(0<<1)|(1<<0);//Establecemos la velocidad de conversión por segundo - 250ksps: Se pone un 1 en binario para indicar que los 2 millos de muestrs por s se divide /8 para obtener 250ksps pág 1159
    ADC0->SSPRI = 0x3210;//Prioridad de los secuenciadores (mayor prioridad en el secuenciador 0) pág 1099
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);//Desactivamos los secuenciadores pág 1077
    ADC0->EMUX  = (0x0000);//Seleccionamos el evento que activa la conversión pág 1091, se configura por procesador 

    //Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado pág 1129
    ADC0->SSMUX0 = (1<<0); //Secuenciador 0 se asignar el canal 1
    ADC0->SSMUX1 = (2<<0); //Secuenciador 1 se asignar el canal 2
    ADC0->SSMUX2 = (8<<0); //Secuenciador 2 se asignar el canal 8

    //Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción pág 1130
    ADC0->SSCTL0 = (1<<2) | (1<<1);
    ADC0->SSCTL1 = (1<<2) | (1<<1);
    ADC0->SSCTL2 = (1<<2) | (1<<1);
    
    //Enable ADC Interrupt
    ADC0->IM |= (1<<0) | (1<<1) | (1<<2); /* Unmask ADC0 sequence 0, 1 y 2 interrupt pag 1082*/
    //Este registro controla la activación de los secuenciadores pág 1077 
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (1<<0); //Habilitamos secuenciadores 1 y 2 
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2);  //Conf. por procesador
}

extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty)
{
    //Habilitamos el modo de configuración
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2);  //Por procesador
      
    while((ADC0->RIS&0x04)==0){};  //Espera al convertidor
       
    //Secuenciador 0 - Canal 1 - PE2
    Result[0] = ADC0->SSFIFO0&0xFFF; // En el FIFO0 se almacenan las muestras del secuenciador 0, por lo que se lee, pag 860 
    duty[0] = (Result[0]*50000)/4096; //Cuentas para 20ms 
    
    //Secuenciador 1 - Canal 2 - PE1
    Result[1] = ADC0->SSFIFO1&0xFFF; // En el FIFO0 se almacenan las muestras del secuenciador 1
    duty[1] = (Result[1]*50000)/4096; //Cuentas para 20ms
     
    //Secuenciador 2 - Canal 8 - PE5
    Result[2] = ADC0->SSFIFO2&0xFFF;// En el FIFO0 se almacenan las muestras del secuenciador 2
    duty[2] = (Result[2]*50000)/4096;//Cuentas para 20ms
    
    ADC0->ISC = (1<<0) | (1<<1) | (1<<2);  //Conversion finalizada

}


