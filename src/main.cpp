#include <mbed.h>

#if !DEVICE_I2CSLAVE
#error [NOT_SUPPORTED] I2C Slave is not supported
#endif

I2CSlave slave(I2C_SDA, I2C_SCL);

int main() {
  char read_buffer[10];
  char write_buffer[10];

  slave.address(0xA0);

  while (1) {
        int i = slave.receive();
        switch (i) {

            // Si le master envoie une requête de lecture
            case I2CSlave::ReadAddressed:
                slave.write(write_buffer, strlen(write_buffer) + 1); // Includes null char
                break;

            // Si le master envoie une requête de lecture générale
            case I2CSlave::WriteGeneral:
                slave.read(read_buffer, 10);
                printf("Read G: %s\n", read_buffer);
                break;

            // Si le master envoie une requête de lecture adressée
            case I2CSlave::WriteAddressed:
                slave.read(read_buffer, 10);
                printf("Read A: %s\n", read_buffer);
                break;
            
            // Dans tous les autres cas
            default:
                printf("Message invalide !\n");
                break;
        }
        
        for (int i = 0; i < sizeof(read_buffer); i++) {
            read_buffer[i] = 0;    // Clear buffer
        }
        
        for (int i = 0; i < sizeof(write_buffer); i++) {
            write_buffer[i] = 0;    // Clear buffer
        }
    }
}