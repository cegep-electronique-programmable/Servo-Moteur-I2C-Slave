#include <mbed.h>

#define ADDRESSE_I2C_PAR_DEFAUT 0x23

#if !DEVICE_I2CSLAVE
#error [NOT_SUPPORTED] I2C Slave is not supported
#endif

// Utiliser la classe I2CSlave pour créer un objet slave.
// Consulter la documentation pour voir les méthodes disponibles.
I2CSlave slave(I2C_SDA, I2C_SCL);

// LED de debug
DigitalOut debug_led(LED1);

// Sortie PWM pour le moteur
PwmOut moteur(PB_4);

// Les états possibles du moteur
enum etat_moteur {Eteint, Allume};

int main() {
    
    printf("Debut du programme servo-moteur-I2C\r\n");

    char read_buffer[10];
    char write_buffer[10];

    for (int i = 0; i < sizeof(write_buffer); i++) {
        write_buffer[i] = 'A';
    }
    write_buffer[9] = '\0';

    // Initialiser le moteur à l'état OFF
    uint8_t etat_moteur = Eteint;

    const int adresse_i2c_7bits = 0x23;
    const int adresse_i2c_8bits = adresse_i2c_7bits << 1; // Nécessairement pair
    slave.address(adresse_i2c_8bits);

    while (1) {

        // Attendre une requête du master
        int i2c_receive = slave.receive();
        //printf("i = 0x%x\r\n", i);

        // Traiter la requête
        switch (i2c_receive) {

            // Si le master envoie une requête de lecture
            case I2CSlave::ReadAddressed:
                printf("ReadAddressed\n");
                ThisThread::sleep_for(100ms);
                ///////////////////////////////////////////
                // Retourner l'état du moteur (sa position ou OFF sous forme d'une chaine de caractères)
                ///////////////////////////////////////////

                //slave.write(write_buffer, strlen(write_buffer) + 1); // Includes null char
                //slave.stop();

                break;

            case I2CSlave::WriteGeneral:
                printf("WriteGeneral\r\n");
                break;

            // Si le master envoie une requête de lecture qui nous est adressée
            case I2CSlave::WriteAddressed:
                printf("WriteAddressed\n");
                slave.read(read_buffer, 10);
                printf("Read A: %s\n", read_buffer);

                int8_t commande_recue = read_buffer[0];

                ///////////////////////////////////////////
                // Modifier l'état du moteur en fonction de la commande reçue
                ///////////////////////////////////////////
                break;

        }
        
        // Vider le buffer de lecture
        for (int i = 0; i < sizeof(read_buffer); i++) {
            read_buffer[i] = 0;
        }
        
        // Vider le buffer d'écriture
        for (int i = 0; i < sizeof(write_buffer); i++) {
            write_buffer[i] = 0;
        }
    }
}