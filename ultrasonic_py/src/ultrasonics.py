#!/usr/bin/env python

import rospy
import RPi.GPIO as GPIO
import time

from geometry_msgs.msg import Twist

class UltrasonicNode:
    def _init_(self):
        # Configurar pines GPIO para los sensores ultrasónicos
        self.pin_trig1 = 14  # GPIO 17 para el primer sensor
        self.pin_echo1 = 15

        self.pin_trig2 = 9  # GPIO 23 para el segundo sensor
        self.pin_echo2 = 10

        self.pin_trig3 = 11   # GPIO 5 para el tercer sensor
        self.pin_echo3 = 19

        GPIO.setmode(GPIO.BCM)
        GPIO.setup([self.pin_trig1, self.pin_echo1, self.pin_trig2, self.pin_echo2, self.pin_trig3, self.pin_echo3],
                   GPIO.OUT)

        # Publicar datos en el tópico /cmd_vel
        self.pub_ult = rospy.Publisher('/ultrasonics', Twist, queue_size=10)

        # Inicializar el nodo de ROS
        rospy.init_node('ultrasonic_node', anonymous=True)

    def read_ultrasonic(self, pin_trig, pin_echo):
        # Generar pulso de trigger
        GPIO.output(pin_trig, True)
        time.sleep(0.00001)
        GPIO.output(pin_trig, False)

        # Medir tiempo de eco
        start_time = time.time()
        while GPIO.input(pin_echo) == 0:
            pulse_start = time.time()
            if pulse_start - start_time > 0.1:
                return -1  # Se ha superado el tiempo máximo

        end_time = time.time()
        while GPIO.input(pin_echo) == 1:
            pulse_end = time.time()

        # Calcular distancia
        pulse_duration = pulse_end - pulse_start
        distance = pulse_duration * 17150  # La velocidad del sonido es aproximadamente 343 metros/segundo

        return distance

    def run(self):
        rate = rospy.Rate(10)  # Frecuencia de publicación (10 Hz)

        while not rospy.is_shutdown():
            # Leer distancias de los sensores
            distance1 = self.read_ultrasonic(self.pin_trig1, self.pin_echo1)
            distance2 = self.read_ultrasonic(self.pin_trig2, self.pin_echo2)
            distance3 = self.read_ultrasonic(self.pin_trig3, self.pin_echo3)

            # Crear mensaje Twist
            cmd_ult_msg = Twist()
            cmd_ult_msg.linear.x = distance1  # Utiliza la distancia del primer sensor para la velocidad lineal
            cmd_ult_msg.linear.y = distance2  # Utiliza la distancia del segundo sensor para la velocidad angular
            cmd_ult_msg.linear.z = distance3  # Utiliza la distancia del segundo sensor para la velocidad angular

            # Publicar datos en el tópico /cmd_vel
            self.pub_ult.publish(cmd_ult_msg)

            rate.sleep()

if _name_ == '_main_':
    try:
        ultrasonic_node = UltrasonicNode()
        ultrasonic_node.run()
    except rospy.ROSInterruptException:
        pass
    finally:
        GPIO.cleanup()  # Limpiar configuraciones GPIO al salir del programa
