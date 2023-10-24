#include <Servo.h>
#include <string.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
#include <time.h>
Servo servo[6];

#define BLUETOOTH_RXD_PIN_NUM 4
#define BLUETOOTH_TXD_PIN_NUM 5

SoftwareSerial bluetoothSerial(BLUETOOTH_RXD_PIN_NUM, BLUETOOTH_TXD_PIN_NUM);

// 시간 제어를 위한 변수
unsigned long asyncStartTime, asyncEndTime;

void (*resetFunc)(void) = 0;
bool set_servoAngle(int servoMotorNumber,
                    int setAngle = 0,
                    int speed = 1,
                    int moveDealyMsec = 30);
void command(String cmd);

bool set_servoAngle(int servoMotorNumber,
                    int setAngle = 0,
                    int speed = 1,
                    int moveDealyMsec = 30)
{
    int run = 1;
    int A_Servo_Value = 0;
    int B_Servo_Value = 0;
    servoMotorNumber -= 1;

    int now_angle = servo[servoMotorNumber].read();
    asyncStartTime = millis();
    while (run)
    {
        asyncEndTime = millis();

        if (asyncEndTime - asyncEndTime >= moveDealyMsec)
        {
            asyncEndTime = millis();
            if (now_angle > setAngle)
            {
                servo[servoMotorNumber].write(now_angle - speed);
                now_angle -= speed;
                A_Servo_Value = B_Servo_Value;
                B_Servo_Value = servo[servoMotorNumber].read();
                if (A_Servo_Value - B_Servo_Value == 0)
                {
                    servo[servoMotorNumber].write(servo[servoMotorNumber].read());
                    break;
                }
                if (now_angle - speed <= setAngle)
                {
                    servo[servoMotorNumber].write(setAngle);
                    bluetoothSerial.print(servo[servoMotorNumber].read() +
                                          (servoMotorNumber + 1) * 1000);
                    Serial.println((int)servo[servoMotorNumber].read() +
                                   (servoMotorNumber + 1) * 1000);
                    break;
                }
            }

            else if (now_angle < setAngle)
            {
                servo[servoMotorNumber].write(now_angle + speed);
                now_angle += speed;
                A_Servo_Value = B_Servo_Value;
                B_Servo_Value = servo[servoMotorNumber].read();
                if (A_Servo_Value - B_Servo_Value == 0)
                {
                    servo[servoMotorNumber].write(servo[servoMotorNumber].read());
                    break;
                }

                if (now_angle + speed >= setAngle)
                {
                    servo[servoMotorNumber].write(setAngle);
                    bluetoothSerial.print(servo[servoMotorNumber].read() + (servoMotorNumber + 1) * 1000);
                    Serial.println((int)servo[servoMotorNumber].read() + (servoMotorNumber + 1) * 1000);
                    break;
                }
            }

            else
            {
                break;
            }
        }
        else
        {
            if (bluetoothSerial.available())
            {
                String data = bluetoothSerial.readString();
                command(data);
                return true;
            }
        }
    }
    return false;
}
int is = 0;

void Hello()
{
    set_servoAngle(2, 90, 1, 30);

    for (int i = 3; i > 0; i--)
    {
        set_servoAngle(3, 50, 1, 30);
        set_servoAngle(3, 90, 1, 30);
    }
}

void Sequins1()
{
    set_servoAngle(1, 90);
    set_servoAngle(5, 155, 1, 10);
    set_servoAngle(2, 35);
    set_servoAngle(3, 135);
    set_servoAngle(4, 15, 1, 10);
    set_servoAngle(5, 10, 1, 10);

    // 물건 잡은 후
    while (1)
    {
        grap(1);
        if (set_servoAngle(2, 50))
            break;
        if (set_servoAngle(1, 140))
            break;
        if (set_servoAngle(2, 40))
            break;
        un_grap(1);

        if (set_servoAngle(2, 50))
            break;
        if (set_servoAngle(1, 90))
            break;
        if (set_servoAngle(2, 40))
            break;
    }
}

void Sequins2()
{
    int angle_motor = servo[0].read();
    int pm = 0;
    set_servoAngle(6, 70, 1, 50);
    set_servoAngle(2, 18, 1, 50);
    set_servoAngle(5, 113, 1, 50);
    set_servoAngle(4, 15, 1, 50);
    set_servoAngle(3, 148, 1, 50);
    set_servoAngle(6, 23, 1, 50);

    while (1)
    {
        grap(1);
        if (set_servoAngle(2, 28))
            break;
        if (set_servoAngle(1, 140))
            break;
        if (set_servoAngle(2, 18))
            break;
        un_grap(1);

        if (set_servoAngle(2, 28))
            break;
        if (set_servoAngle(1, 90))
            break;
        if (set_servoAngle(2, 18))
            break;
    }
}

void Sequins3()
{
    set_servoAngle(2, 180, 1, 50);
    set_servoAngle(3, 180, 1, 50);
    set_servoAngle(4, 180, 1, 50);
    set_servoAngle(5, 9, 1, 50);
    for (int i = 0; i < 3; i++)
    {
        set_servoAngle(6, 15, 1, 50);
        set_servoAngle(6, 70, 1, 50);
    }
}

void grap(uint8_t speed)
{
    set_servoAngle(6, 15, speed);
}

void un_grap(uint8_t speed)
{
    set_servoAngle(6, 75, speed);
}

void setup()
{
    Serial.begin(115200);
    bluetoothSerial.begin(115200);
    Serial.setTimeout(1);
    bluetoothSerial.setTimeout(1);
    Serial.println("Hello, world!");

    servo[0].attach(2);
    servo[1].attach(3);
    servo[2].attach(6);
    servo[3].attach(9);
    servo[4].attach(10);
    servo[5].attach(11);

    for (int i = 1; i < 7; i++)
    {
        if (i == 6)
        {
            set_servoAngle(6, 45, 1, 50);
        }

        else
        {
            set_servoAngle(i, 90, 1, 50);
        }
    }
}

void command(String cmd)
{
    Serial.println("Received Data: " + String(cmd));
    if (cmd == "안녕" || cmd == "Hello" || cmd == "안녕하세요")
    {
        Hello();
    }
    else if (cmd == "reset")
    {
        resetFunc();
    }
    else
    {
        int intValue = cmd.toInt();

        switch (intValue / 1000)
        {
        case 1:
            set_servoAngle(1, intValue % 1000, 1, 15);
            break;

        case 2:
            set_servoAngle(2, intValue % 1000, 1, 15);
            break;

        case 3:
            set_servoAngle(3, intValue % 1000, 1, 15);
            break;

        case 4:
            set_servoAngle(4, intValue % 1000, 1, 15);
            break;

        case 5:
            set_servoAngle(5, intValue % 1000, 1, 15);
            break;

        case 6:
            set_servoAngle(6, intValue % 1000, 1, 15);
            break;

        case 7:
            Sequins1();
            break;

        case 8:
            Sequins2();
            break;

            // case 9:
            //     Sequins3();
            //     break;

        default:
            break;
        }
    }
}

void loop()
{
    if (bluetoothSerial.available())
    {
        String data = bluetoothSerial.readString();
        command(data);
    }
}
