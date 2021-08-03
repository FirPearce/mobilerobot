#define pwmbelkiri 8 //roda kiri belakang
#define dirbelkiri 9 //roda kiri belakang
#define pwmbelkanan 10 //roda kanan belakang
#define dirbelkanan 11 //roda kanan belakang
#define button 2 //menyalakan mobil 
#define sensdepan A0 //ultrasonic depan
#define senskiri A2 //ultrasonic kiri
#define senskanan A3 //ultrasonic kanan
#define sensbelakang A1 //ultrasonic belakang
#define servoarah1 23 // arah roda kiri
#define servoarah2 24 // arah roda kanan
#define servosemprot 22 // arah semprot servo
#define suhu A4 //sensor suhu
#define kecepatan A5 //sensor kecepatan
#define batt1 26 // penanda baterai
#define batt2 27 // penanda baterai
#define batt3 28 // penanda baterai
#define batt4 29 // penanda baterai
#define gyro_x 4 // button gyro
#define gyro_y 5 // button gyro

#include "Adafruit_LiquidCrystal.h"
Adafruit_LiquidCrystal lcd(0x20); //address lcd

int baterai; //variabel batre
int i; //deklarasi iterasi i
int depan,belakang,kiri,kanan;
int dutycycle1 = 1500; //lurus
int dutycycle2 = 2500; //kiri
int dutycycle3 = 500; //kanan
int dutycycle4 = 2000; //diagonal kri
int dutycycle5 = 1000; //diagonal kanan
int perioda = 18000;
int push, nilaisuhu, x,y,cepat,nilai,nilai_baterai; // x y untuk menyimpan input button gyro
String tentuarah, sementara, judul;
int buzzer = 3;
int gyro = 0;

void setup()
{
	pinMode(button, INPUT);
	pinMode(gyro_x, INPUT); // gyro miring kanan
	pinMode(gyro_y, INPUT); //gyro miring kiri
	pinMode(pwmbelkiri, OUTPUT);
	pinMode(dirbelkiri, OUTPUT);
	pinMode(pwmbelkanan, OUTPUT);
	pinMode(dirbelkanan, OUTPUT);
	pinMode(servoarah1, OUTPUT);
	pinMode(servoarah2, OUTPUT);
	pinMode(servosemprot, OUTPUT);
	pinMode(buzzer, OUTPUT);
	pinMode(batt1, OUTPUT);
	pinMode(batt2, OUTPUT);
	pinMode(batt3, OUTPUT);
	pinMode(batt4, OUTPUT);
	lcd.begin(20,2);//20 kolom 2 baris
	lcd.setBacklight(HIGH);
	Serial.begin(9600);
	baterai = 100;
	i = 0;
}

void loop()
{
	depan = analogRead(sensdepan)/4; //baca sensor ultrasonic depan
	belakang = analogRead(sensbelakang)/4; //baca sensor ultrasonic belakang
	kiri = analogRead(senskiri)/4; //baca sensor ultrasonic kiri
	kanan = analogRead(senskanan)/4; //baca sensor ultrasonic kanan
	nilaisuhu = analogRead(suhu)/17;
	cepat = analogRead(kecepatan)/4;
	x = digitalRead(gyro_x); // input gyro miring kanan
	y = digitalRead(gyro_y); //input gyro miring kiri
	on(); //menyalakan function on
}

void on(){
	push = digitalRead(button); //baca inputan button
	if (push == HIGH){	//kalau button ditekan
		String temp = String(cepat/10)+ "CM/S  ";
		if(i==80){
			lcd.setCursor(0,0);
			lcd.print("Kecepatan         ");
			lcd.setCursor(0,1);
			lcd.print(temp);
		}
		analogWrite(pwmbelkiri, cepat); //maka nyala motor dc
		digitalWrite(dirbelkiri, LOW);
		analogWrite(pwmbelkanan, cepat);
		digitalWrite(dirbelkanan, LOW);
		batre(baterai);
		belok();
		suhu(nilaisuhu);
		miring(gyro);
	}else{	// kalau button tidak ditekan
		digitalWrite(pwmbelkiri, LOW); //mati
		digitalWrite(pwmbelkanan, LOW);
		digitalWrite(servoarah1, LOW);
		digitalWrite(servoarah2, LOW);
	}
}

void batre( int nilai_baterai){
	String temp = String(nilai_baterai)+ "%   ";
	if(i==20){ 
		lcd.setCursor(0,0);
		lcd.print("Kondisi Baterai   ");
		lcd.setCursor(0,1);
		lcd.print(temp);
	}
	nilai_baterai = baterai;
	if (baterai !=0){	//jika baterai tidak = 0
		i= i+1; //iterasi bertambah 1		
		if ((i==100) && (baterai!=0)) {	//jika i=170 dan batre masih banyak
			baterai=baterai-1; //batre bekurang
			i=0; //indeks kembali ke 0
			//delay(500); //delay
			if(baterai >=70 && baterai<=100){	//kalau baterai >=70 && baterai<=100
				digitalWrite(batt1, HIGH);//indikator led nyala semua
				digitalWrite(batt2, HIGH);
				digitalWrite(batt3, HIGH);
				digitalWrite(batt4, HIGH);
			}else if(baterai >=40 && baterai <70){	//kalau baterai >=40 && baterai <70
				digitalWrite(batt1, HIGH);//indikator led mati satu
				digitalWrite(batt2, HIGH);
				digitalWrite(batt3, HIGH);
				digitalWrite(batt4, LOW);
			}else if(baterai >=10 && baterai <40){	// kalau baterai >=10 && baterai <40		
				digitalWrite(batt1, HIGH);//indikator led mati dua
				digitalWrite(batt2, HIGH);
				digitalWrite(batt3, LOW);
				digitalWrite(batt4, LOW);
			}else if(baterai >=1 && baterai <10){	//kalau baterai >=1 && baterai <10
				digitalWrite(batt1, HIGH);//indikator led mati tiga
				digitalWrite(batt2, LOW);
				digitalWrite(batt3, LOW);
				digitalWrite(batt4, LOW);
			}else{	//kalau habis batre
				digitalWrite(batt1, LOW);//indikator led mati semua
				digitalWrite(batt2, LOW);
				digitalWrite(batt3, LOW);
				digitalWrite(batt4, LOW);
			}
		}
	}else{	//ketika habis batre
		digitalWrite(pwmbelkiri, LOW); //mati
		digitalWrite(pwmbelkanan, LOW);
	}

}

void miring(int nilai_gyro){
	String temp = String(nilai_gyro)+ "^o  ";
	if (i==40){
		lcd.setCursor(0,0);
		lcd.print("Kondisi Kemiringan");
		lcd.setCursor(0,1);
		lcd.print(temp);
	}
	nilai_gyro = gyro;
	if (gyro>=-40 && gyro<=40){	//kalau gyro nya lebih dari -40 dan gyro kurang dari 4-
		if(x == HIGH){	//button x
			gyro = gyro + 10; //menambah nilai gyro sebagai simulasi miring
		}if(y == HIGH){	//buttton y
			gyro = gyro - 10;//mengurangi nilai gyro sebagai simulasi miring
		}
	}else{	//jika tidak terpenuhi
		Serial.println("TERLALU MERENG");//print ini
		gyro = 0; //reset gyro ke 0
	}	
}
void suhu(int nilai_suhu){
	String temp = String(nilai_suhu)+ "^o  ";
	if (i==60){
		lcd.setCursor(0,0);
		lcd.print("Kondisi Suhu Mesin");
		lcd.setCursor(0,1);
		lcd.print(temp);
	}
	nilai_suhu = nilaisuhu;
	if (nilaisuhu>=60){	// kalau suhu nya dah maksimal
		tone(buzzer, 500); //nyalain buzzer
	}else{	//kalau kondisi atas tidak terpenuhi
		noTone(buzzer); //buzzer diam
	}
}

void belok(){
	if (depan>=150 && belakang>=150 && kiri>=150 && kanan>=150){	//jika ada benda di sekitar robot
		digitalWrite(pwmbelkiri, LOW); // maka robot diam
		digitalWrite(pwmbelkanan, LOW);	
		digitalWrite(servoarah1, HIGH); //servo 1 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servoarah2, HIGH); //servo 2 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servosemprot, HIGH); //servo semprot lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle1);	
	}if (depan==0 && belakang==0 && kiri==0 && kanan==0){	//jika tidak ada benda
		digitalWrite(servoarah1, HIGH);//maju
		delayMicroseconds(dutycycle1);//servo 1 lurus
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servoarah2, HIGH); //servo 2 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servosemprot, HIGH); //servo semprot lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle1);	
	}if (depan>=150 && belakang==0 && kiri==0 && kanan==0){	//jika benda di depan
		digitalWrite(servoarah1, HIGH);//servo 1 belok ke kiri
		delayMicroseconds(dutycycle2);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle2);
		digitalWrite(servoarah2, HIGH); // servo 2 belok ke kiri
		delayMicroseconds(dutycycle2);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle2);
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle1);
	}if (depan>=150 && kiri>=150 && belakang==0 && kanan==0){	//jika benda di depan&kiri
		digitalWrite(servoarah1, HIGH); //servo 1 belok kanan
		delayMicroseconds(dutycycle3);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle3);
		digitalWrite(servoarah2, HIGH); // servo 2 belok kanan
		delayMicroseconds(dutycycle3);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle3);
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot diagonal kiri
		delayMicroseconds(dutycycle4);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle4);
	}if (depan>=150 && kiri==0 && belakang==0 && kanan>=150){	//jika benda di depan&kanan
		digitalWrite(servoarah1, HIGH); //servo 1 belok kiri
		delayMicroseconds(dutycycle2);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle2);
		digitalWrite(servoarah2, HIGH); //servo 2 belok kiri
		delayMicroseconds(dutycycle2);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle2);
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot diagonal kanan
		delayMicroseconds(dutycycle5);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle5);
	}if (depan==0 && belakang==0 && kiri >=150 && kanan==0){	//jika benda di kiri
		digitalWrite(servoarah1, HIGH); //servo 1 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servoarah2, HIGH); //servo 2 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot kiri
		delayMicroseconds(dutycycle2);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle2);
	}if (depan==0 && belakang==0 && kiri ==0 && kanan>=150){	//jika benda di kanan
		digitalWrite(servoarah1, HIGH); //servo 1 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servoarah2, HIGH); //servo 2 lurus
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot kiri
		delayMicroseconds(dutycycle3);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle3);
	}if (depan==0 && kiri>=150 && belakang==0 && kanan>=150){	//jika benda di depan&kanan
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot diagonal kanan
		delayMicroseconds(dutycycle2);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle2);
		digitalWrite(servosemprot, HIGH); //servo semprot menyemprot diagonal kanan
		delayMicroseconds(dutycycle3);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle3);
	}else{	//selain itu
		digitalWrite(servoarah1, LOW);//servo 1 mati
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah1, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servoarah2, LOW);//servo 2 mati
		delayMicroseconds(dutycycle1);
		digitalWrite(servoarah2, LOW);
		delayMicroseconds(perioda-dutycycle1);
		digitalWrite(servosemprot, LOW); //servo semprot mati
		delayMicroseconds(dutycycle1);
		digitalWrite(servosemprot, LOW);
		delayMicroseconds(perioda-dutycycle1);
	}
}