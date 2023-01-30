#include "lib/include.h"

//Experimento 2
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 cuya frecuencia sea de 50Hz con tres potenciometros variar el ciclo de trabajo
 para controlar la posicion de tres servos sg90 o otros */

int main(void)
{

    //Funciones para configuración 
    Configurar_PLL(); 
    //Configurar_UART0();
    Configura_Reg_PWM0(8);
    Configura_Reg_ADC0();

    while(1) 
    {
        //Se llama a la función para leer y convertir valores del ADC
        ADC0_InSeq2(Result,duty);
      
    
    }
}
