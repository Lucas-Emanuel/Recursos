/* Software de uso livre
   ___________________
   >> NÃO COMERCIAL <<
   ===================
   
   Desenvolvido por Lucas Emanuel Lobo Costa
   Github: Lucas-Emanuel/Recursos
   
   -Programa desenvolvido para a leitura de botões em microcontroladores 
    > Anti bounce
    > Detécta a borda de subida/descida evitando leituras erradas


*/


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-/DEFINIÇÕES/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

#define botao    13

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/VARIÁVEIS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

boolean Clock[1],
        ok[1],
        botaoant[1],
        timeron[1];       

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void setup() {
 //-=-=-=-=-=-=-=-=-=-=-=-=-/configuração do timer 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|     
     
     TCCR2A = 0x00;   //Timer operando em modo normal
     TCCR2B = 0x07;   //Prescaler 1:1024
     TCNT2  = 100;    //10 ms overflow again
     TIMSK2 = 0x01;   //Habilita interrupção do Timer2

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/DEFINIÇÃO DE PINOS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

 pinMode(botao , INPUT_PULLUP );                 // configura o botão 1 como entrada com pull-up interno ligado

 //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

}//end setup//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void loop() {



}//end loop/-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ISR(TIMER2_OVF_vect)
{
    TCNT2=100;          // Reinicializa o registrador do Timer2

//===========/Clock 1/============//50ms
    
    if(timeron[1]){          //Se timeron[1] for verdadeiro
      counter[1]++;          //counter[1] é incrementado a cada estouro do timer
      }
    
    else{
        counter[1] = 0;
        Clock[1] = false;    //Torna clock[1] falso
        }           
    
    if(counter[1] >= 5){     //Se counter[1] for maior ou igual a 5
      Clock[1] = true;       //Clock[1] se torna verdadeiro
      counter[1] = 0;        //counter[1] é reinicializado
      }
    
}//end clock/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void leitura_dos_botoes() {   
   
   
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/LEITURA BOTÃO 1/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

     
//   if (digitalRead(botao) && (digitalRead(botao) != botaoant[1])){          //Detecta a borda de subida no sinal do botao
   if (!digitalRead(botao) && (digitalRead(botao) != botaoant[1])){         //Detecta a borda de descida no sinal do botao
      timeron[1] = true;                                                    //Liga o Clock 1
      ok[1] = true;                                                         //OK se torna verdadeiro
     }//end if
     
     if (ok[1] && Clock[1]){                                                //aguarda o estouro do Clock 1 para o cancelamento do efeito bounce
      timeron[1] = false;                                                   //Desliga o Clock 1
      ok[1] = false;                                                        //OK se torna verdadeiro
      // CODE IF
     
      
     }//end if     
     else{
      // CODE ELSE
             
     
     }//end else
        
     botaoant[1] = digitalRead(botao) ;                              // atribui à variável (botaoant[1]) o valor de (botaoatu[1])
  
}//end input//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
