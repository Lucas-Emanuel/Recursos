
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-/DEFINIÇÕES/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

#define botaol    13
#define botaor    11
#define botaook   12

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/VARIÁVEIS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

boolean Clock[7],
        ok[3],
        botaoatu[3],
        botaoant[3],
        timeron[7],        
        longpress,
        shortpress,
        pagina[4],
        travamenu,
        voltar,
        zero[2],
        blackout,
        oldblackout,
        botao[3];
            

int debug[6],
    counter[7];

byte retorno,
     menus = 0,     
     fase = 0,
     menusconfig[5];
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void setup() {
 //-=-=-=-=-=-=-=-=-=-=-=-=-/configuração do timer 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|     
     
     TCCR2A = 0x00;   //Timer operando em modo normal
     TCCR2B = 0x07;   //Prescaler 1:1024
     TCNT2  = 100;    //10 ms overflow again
     TIMSK2 = 0x01;   //Habilita interrupção do Timer2

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/DEFINIÇÃO DE PINOS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

 pinMode(botaol    , INPUT_PULLUP );                 // configura o botão 1 como entrada com pull-up interno ligado
 pinMode(botaor    , INPUT_PULLUP );                 // configura o botão 2 como entrada com pull-up interno ligado
 pinMode(botaook   , INPUT_PULLUP );                 // configura o botão 3 como entrada com pull-up interno ligado

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
        Clock[1] = false;   //Torna clock[1] falso
        }           
    
    if(counter[1] >= 5){     //Se counter[1] for maior ou igual a 5
      Clock[1] = true;       //Clock[1] se torna verdadeiro
      counter[1] = 0;        //counter[1] é reinicializado
      }
    
}//end clock/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void leitura_dos_botoes() {   
   
   
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/LEITURA BOTÃO 1/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   
   
     botao[1] = false;
     botaoatu[1] = digitalRead(botaol);                       //Lê o valor do botão1 e o atribui a variável (botaoatu[1])

     if (!botaoatu[1] && (botaoatu[1] != botaoant[1])){       //Detecta a borda de descida no sinal do botao1
      timeron[1] = true;
      ok[1] = true;
     }
     
     if (ok[1] && Clock[1]){                                  //Detecta a borda de descida no sinal do botao1
      timeron[1] = false;
      ok[1] = false;
      // CODE IF
     
      
     }//end if     
     else{
      // CODE ELSE
             
     
     }//end else
        
     botaoant[1] = botaoatu[1] ;                              // atribui à variável (botaoant[1]) o valor de (botaoatu[1])
  
}//end input//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
