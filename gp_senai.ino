/*  2- display
 *  4- display
 *  5- display 
 *  6- display
 *  7- display
 *  13- botão
 *  12- botão
 *  11- botão
 *  
 *  
 *  
 *  principal:
 *            consumo: ___kw
 *            autonomia: __h__min
 *            
 *            bateiras: ____v  ___%
 *            
 *            rede: ___v
 *            inversor: ___v
 *            
 *            sistema: on/off + para ligar
 *            
 *            blackout: on/off + para ligar
 *  
 *  configurações:  inversor:
 *                           parametros: 
 *                                      potência: ___w
 *                                      tensão: 127/220v
 *                           delay off/on:
 *                                        delay: __s
 *                           
 *                  chaveamento:
 *                              nivel A:
 *                                      potência: ___w
 *                                      tolerância: __s
 *                                      função: A/B/C     A= chaveamento / B= cooler / C= buzzer
 *                              nivel B:
 *                                      potência: ___w
 *                                      tolerância: __s
 *                                      função: A/B/C
 *                              nivel C:
 *                                      potência: ___w
 *                                      tolerância: __s
 *                  bateria:
 *                          tensão maxima: __v
 *                  blackout:








*/
#include <EEPROM.h>
#include <LiquidCrystal.h>
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-/DEFINIÇÕES/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

#define botaol    13
#define botaor    11
#define botaook   12

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/VARIÁVEIS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

boolean Clock[10],
        ok[3],
        botaoatu[3],
        botaoant[3],
        timeron[10],        
        longpress,
        shortpress,
        pagina[4],
        travamenu,
        voltar,
        zero[2],
        blackout,
        oldblackout,
        botao[4];
            

int debug[10],
    menus = 0,
    counter[10];

byte retorno,    
     fase = 0;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-/PARA USO BIBLIOTECAS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|          

LiquidCrystal lcd(8,7,6,5,4,2);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/FUNÇÕES/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

void leitura_dos_botoes();
void tela(int menustela);
void configuracao(int menusconfig);


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void setup() {
  
 
//-=-=-=-=-=-=-=-=-=-=-=-=-/configuração do timer 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|     
     
     TCCR2A = 0x00;   //Timer operando em modo normal
     TCCR2B = 0x07;   //Prescaler 1:1024
     TCNT2  = 100;    //10 ms overflow again
     TIMSK2 = 0x01;   //Habilita interrupção do Timer2

//=-=-=-=-=-=-=-=-=-=-=-=-=-/DEFINIÇÃO DE PARAMETROS/-=-=-=-=-=-=-=-=-=-=-=-=-=-=|     
 
 Serial.begin(9600);                                  // inicia a porta serial, configura a taxa de dados para 9600 bps
 
 lcd.begin(16,2);                                    // configuração do tamanho do display lcd

 //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/DEFINIÇÃO DE PINOS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

 pinMode(botaol    , INPUT_PULLUP );                 // configura o botão 1 como entrada com pull-up interno ligado
 pinMode(botaor    , INPUT_PULLUP );                 // configura o botão 2 como entrada com pull-up interno ligado
 pinMode(botaook   , INPUT_PULLUP );                 // configura o botão 3 como entrada com pull-up interno ligado
 pinMode(A0        , OUTPUT       );
 pinMode(3        , OUTPUT       );
 pinMode(A4        , OUTPUT       );
 //=-=-=-=-=-=-=-=-=-=-=-=-=-=/DEFINIÇÃO DE ESTADOS/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|

 pagina[1] = true;
 pagina[2] = false;
 pagina[3] = false;
 blackout  = EEPROM.read(0);
 
 
  //lcd.clear();
  //lcd.home();
  //lcd.print("|>  ArduSun   <|");

  delay(100);

 

 }//end setup//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void loop() {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/FUNÇÕES/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=| 

   leitura_dos_botoes();

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-/DEBUG/-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   

   Serial.print(pagina[1]);                                        // informa a IDE o conteúdo de uma variável
   Serial.print("____");
   Serial.print(pagina[2]);
   Serial.print("____");
   Serial.print(pagina[3]);
   Serial.print("____");
   Serial.print(menus);
   Serial.print("___");
   Serial.print(botao[1]);
   Serial.print("____");
   Serial.print(debug[2]);
   Serial.print("____");
   Serial.print(debug[3]);
   Serial.print("____");
   Serial.print(debug[4]);
   Serial.print("____");
   Serial.println(blackout);

//========================/Tela/========================//  
   if(pagina[1] && !pagina[2] && !pagina[3]){
    
    if(menus > 1) menus = 0;
    if(menus < 0) menus = 1;
    
    tela(menus);
   }
   
//====================/Configuração/====================//  
   if(!pagina[1] && pagina[2] && !pagina[3]){

    if(menus > 3) menus = 0;
    if(menus < 0) menus = 3;

    configuracao(menus);
   }
   
//======================================================//  
   if(longpress && pagina[1]){    
      
     pagina[1] = false;
     pagina[2] = true;
     pagina[3] = false;
     menus = 0;
     longpress = false;
     shortpress = false;
     botao[1] = false;
     botao[3] = false;
      
     }

//==================/Config. Inversor/==================// 

   if(!pagina[1] && !pagina[2] && pagina[3]){

    if(menus > 2) menus = 0;
    if(menus < 0) menus = 2;

    configuracao(menus);
   }
   
//======================================================//
   if(shortpress && pagina[2] && (menus == 0)){    
      
     pagina[1] = false;
     pagina[2] = false;
     pagina[3] = true;
     menus = 0;
     shortpress = false;
      
     }
//======================/Blackout/======================//
   if(pagina[1] && shortpress && (menus == 3)){
      blackout = !blackout;
      EEPROM.write(0, blackout);
      shortpress = false;
    }
//================/Voltar ao principal/=================//   
   if(shortpress && pagina[2] && (menus == 3)){    
      
     pagina[1] = true;
     pagina[2] = false;
     pagina[3] = false;
     menus = 0;
     shortpress = false;
      
     }      
//=============/Voltar as configurações/===============//   
   if(shortpress && pagina[3] && (menus == 2)){    
      
     pagina[1] = false;
     pagina[2] = true;
     pagina[3] = false;
     menus = 0;
     shortpress = false;
      
     }
//================/Regarga destravar/=================//   
   if(shortpress && pagina[3] && (menus == 0)){    
      
     timeron[6] = true;
     timeron[7] = true;
     digitalWrite(A0,HIGH);
     
      
     }
     
    
//================/Regarga destravar/=================//   
   if(shortpress && pagina[3] && (menus == 1)){    
      
     pagina[4] = true;
     pagina[1] = false;
     pagina[2] = false;
     pagina[3] = false;//=================================================================================================================================================================
      
     }
}//end loop/-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ISR(TIMER2_OVF_vect)
{
    TCNT2=100;          // Reinicializa o registrador do Timer2

//===========/Clock 1/============//60ms
    
    counter[1]++;            //Incremena counter[1] a cada estouro do timer
    
    Clock[1] = false;        //Torna clock[1] falso

    if(counter[1] >= 6){    //Se counter[1] for maior ou igual a 10
      Clock[1] = true;       //Clock[1] se torna verdadeiro
      counter[1] = 0;        //counter[1] é reinicializado
      }

//===========/Clock 2/============//50ms    

    if(timeron[2]){          //Se timeron[2] for verdadeiro
      counter[2]++;          //counter[2] é incrementado a cada estouro do timer
      }
    
    else{
        counter[2] = 0;
        Clock[2] = false;   //Torna clock[2] falso
        }           
    
    if(counter[2] >= 5){     //Se counter[2] for maior ou igual a 5
      Clock[2] = true;       //Clock[2] se torna verdadeiro
      counter[2] = 0;        //counter[2] é reinicializado
      }

//===========/Clock 3/============//2000ms

    if(timeron[3]){          //Se timeron[3] for verdadeiro
      counter[3]++;          //counter[3] é incrementado a cada estouro do timer
      }
      else{
        counter[3] = 0;
        Clock[3] = false;
        }

    if(counter[3] > 200){    //Se counter[3] for maior ou igual a 6
      Clock[3] = true;       //Clock[3] se torna verdadeiro
      counter[3] = 0;        //counter[3] é reinicializado  
      }

//===========/Clock 4/============//50ms

    if(timeron[4]){          //Se timeron[4] for verdadeiro
      counter[4]++;          //counter[4] é incrementado a cada estouro do timer
      }
      else{
        counter[4] = 0;
        Clock[4] = false;
        }

    if(counter[4] > 50){    //Se counter[4] for maior ou igual a 6
      Clock[4] = true;       //Clock[4] se torna verdadeiro
      counter[4] = 0;        //counter[4] é reinicializado  
      }

//===========/Clock 5/============//50ms

    if(timeron[5]){          //Se timeron[5] for verdadeiro
      counter[5]++;          //counter[5] é incrementado a cada estouro do timer
      }
      else{
        counter[5] = 0;
        Clock[5] = false;
        }

    if(counter[5] > 50){    //Se counter[5] for maior ou igual a 6
      Clock[5] = true;       //Clock[5] se torna verdadeiro
      counter[5] = 0;        //counter[5] é reinicializado  
      }

//===========/Clock 6/============//2000ms

    if(timeron[6]){          //Se timeron[5] for verdadeiro
      counter[6]++;          //counter[5] é incrementado a cada estouro do timer
      }
    else{
      counter[6] = 0;
      Clock[6] = false;
        }

    if(counter[6] > 200){    //Se counter[5] for maior ou igual a 6
      Clock[6] = true;       //Clock[5] se torna verdadeiro
      counter[6] = 0;        //counter[5] é reinicializado
      timeron[6] = false;
      timeron[7] = false;
      digitalWrite(A0,LOW);  
      }
//===========/Clock 7/============//X-ms

    if(timeron[7]){          //Se timeron[5] for verdadeiro
      counter[7]++;          //counter[5] é incrementado a cada estouro do timer
      }
    else{
      counter[7] = 0;
      Clock[7] = false;
        }

    if(counter[7] > 5){    //Se counter[5] for maior ou igual a 6
      Clock[7] = true;       //Clock[5] se torna verdadeiro
      counter[7] = 0;        //counter[5] é reinicializad
      analogWrite(3,125);       
      }else Clock[7] = false;
      
}//end clock/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void leitura_dos_botoes() {   
   
   
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/LEITURA BOTÃO 1/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   
   
     botao[1] = false;
     botaoatu[1] = digitalRead(botaol);                       //Lê o valor do botão1 e o atribui a variável (botaoatu[1])

     if (!botaoatu[1] && (botaoatu[1] != botaoant[1])){       //Detecta a borda de descida no sinal do botao1
      timeron[4] = true;
      ok[1] = true;
     }
     
     if (ok[1] && Clock[4]){                                  //Detecta a borda de descida no sinal do botao1
      timeron[4] = false;
      ok[1] = false;
      botao[1]= true;                                         //botao1 se torna verdadeiro
      menus--;
     }
        
     botaoant[1] = botaoatu[1] ;                              // atribui à variável (botaoant[1]) o valor de (botaoatu[1])
  

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/LEITURA BOTÃO 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   
   
   
     botaoatu[2] = digitalRead(botaook);                      //Lê o valor do botão1 e o atribui a variável (botaoatu[2])
     if(!botaoatu[2] && (botaoatu[2] != botaoant[2])){        //Detecta a borda de descida no sinal do botao2
        ok[2] = true;
        timeron[2] = true;      
     }
     
     if(ok[2] && Clock[2]){                                   //Detecta a borda de descida no sinal do botao2
        timeron[2] = false;
        ok[2] = false;
        fase = 1;                                             //Muda o estado de fase para 1
        timeron[3] = true;                                    //Ativa a contagem do Clock[3]        
        
     }    

     longpress = false;
     if(!digitalRead(botaook) && Clock[3] && (fase == 1)){    //Se o botao2 continua precionado depois de 5000 ms
        timeron[3] = false;                                   //Desativa a contagem do Clock[3]
        fase = 0;                                             //Fase volta a valer 0
        //=================
        debug[2]++;                                           //Incrementa debug[2] em 1
        longpress = true;                                     //Longpress se torna verdadeiro     
                
      }
     
     shortpress = false;
     
     if(botaoatu[2] && (botaoatu[2] != botaoant[2])){
      if(fase == 1){
         shortpress = true;
         debug[1]++;
         //menus++;
         timeron[3] = false;
         fase = 0;
        }
             
      }
       
     botaoant[2] = botaoatu[2] ;                             // atribui à variável (botaoanteriorOK) o valor de (botaoatualOK)     

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/LEITURA BOTÃO 3/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   
   
     botao[3] = false;
     botaoatu[3] = digitalRead(botaor);                       //Lê o valor do botão1 e o atribui a variável (botaoatu[1])

     if (!botaoatu[3] && (botaoatu[3] != botaoant[3])){       //Detecta a borda de descida no sinal do botao1
      timeron[5] = true;
      ok[3] = true;
      }
     
     if (ok[3] && Clock[5]){                                  //Detecta a borda de descida no sinal do botao1
      timeron[5] = false;
      ok[3] = false;
      botao[3]= true;                                         //botao1 se torna verdadeiro
      menus++;
     }
        
     botaoant[3] = botaoatu[3] ;                              // atribui à variável (botaoant[1]) o valor de (botaoatu[1])

   
}//end input//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void tela(int menustela) {

  if(pagina[1]){

   switch(menustela) {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 0/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   
    
    case 0:

    if(Clock[1]){
                lcd.clear();
                lcd.noBlink();
                lcd.print("Quantidade de");
                lcd.setCursor(0,1);
                lcd.print("Comprimidos:");
                lcd.setCursor(13,1);
                lcd.print("XXX");
                           

    };//close if

    break;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 1/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|   
    
    case 1:

    if(Clock[1]){
                lcd.clear();
                lcd.noBlink();
                lcd.print("Ejecao Manual");
                lcd.setCursor(0,1);
                lcd.print("Pressione ok");
                           

    };//close if

    break;

    
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página x/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|    
    default:

    lcd.clear();

    break;
    
    }  
  }
}//end tela//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void configuracao(int menusconfig) {
  

  if(pagina[2]){

   switch(menusconfig){

    case 0:    
    
     if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("|>  Recarga   <|");
      lcd.setCursor(0,1);
      lcd.print("<1>>>>>>>>>>>>>>");         
      };

     
    break;
    
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 1/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    case 1:

     if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("|> Velocidade <|");
      lcd.setCursor(0,1);
      lcd.print("<<2>>>>>>>>>>>>>");      
      };//end if clock[1]

    break;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    case 2:

    if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("|>    PPC     <|");
      lcd.setCursor(0,1);
      lcd.print("<<<3>>>>>>>>>>>>");       
      };    
     

    break;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    case 3:

    if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("|>   Voltar   <|");
      lcd.setCursor(0,1);
      lcd.print("<<<<4>>>>>>>>>>>");       
      };    
     

    break;

    default:

     lcd.clear();

    break; 
 
  };//end switch/=-=-=-=-=-=-=-=-=-=-=-=-=-| 
  };//end if travatela/=-=-=-=-=-=-=-=-=-=| 

//=-=-=-=-=-=/configuração do inversor/=-=-=-=-=-=//
  
  if(pagina[3]){

   switch(menus){
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 0/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    case 0:

     if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("----Regarga-----");
      lcd.setCursor(0,1);
      lcd.print("1-Destravar: ok");
      lcd.setCursor(0,1);
      lcd.blink();
      
     }

    break;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 1/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    case 1:    
    
     if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("1-Destravar: ok");
      lcd.setCursor(0,1);
      lcd.print("2-Quantidade:");
      lcd.setCursor(13,1);
      lcd.print("XXX");
      lcd.setCursor(0,1);
      lcd.blink();     
      
     }

     
    break;
    
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página 2/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    case 2:

     if(Clock[1]){
      lcd.noBlink();
      lcd.clear();
      lcd.home();
      lcd.print("2-Quantidade:");
      lcd.setCursor(13,0);
      lcd.print("XXX");
      lcd.setCursor(0,1);
      lcd.print("#-Voltar");
      lcd.setCursor(0,1);
      lcd.blink();     
      
     }

    break;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=/Página x/=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
    default:

    lcd.clear();

    break;
    
  };//end switch
  };//end if travatela

  }//end configuração//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
