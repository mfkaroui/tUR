#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "hasp_types.h"
#include "goat_funcs.h"
#include "master_globals.h"


void setupMasterSerials( void )
{    //Serial to HASP
    Serial.begin( 1200 );
    while( !Serial );

    //Serial to Slave
    Serial1.begin( 300 );
    while( !Serial1 );
}

void setupMasterGlobals( void )
{
    log_name = getNextFile( "GOAT" );
    pump_on = false;
    take_readings = true;
    ground_index = 0;
    slave_index = 0;

    //clear these out appropriately
    memset( &current_gtp.data[0], 0, sizeof( current_gtp.data ) );
    memset( &master_reading.time[0], ' ', sizeof( master_reading ) - 4 );
    memset( &slave_reading.time[0], ' ', sizeof( slave_reading ) - 4 );
    memset( &receive_buffer_ground[0], 0, MAX_BUF );
    memset( &receive_buffer_slave[0], 0, MAX_BUF );

    //setup the sensor reading message as its how we communicate 
    assignEntry( master_reading.time, C_TIME(), sizeof( master_reading.time ) );
    assignEntry( master_reading.bank, "1", sizeof( master_reading.bank ) );
    assignEntry( master_reading.pump_status, "PUMP OFF", sizeof( master_reading.pump_status ) );
    assignEntry( master_reading.reading_status, "FIRST", sizeof( master_reading.reading_status ) );
}

void setupMasterSensors( void )
{
    //Setup the SD
    if( !SD.begin( SD_PIN ) )
       assignEntry( master_reading.sd_status, "SD FAILED", sizeof( master_reading.sd_status ) );
    else
       assignEntry( master_reading.sd_status, "SD GOOD", sizeof( master_reading.sd_status ) );

    //Setup the BME
    if( !bme.begin() )
       assignEntry( master_reading.temp_reading, "BFAIL", sizeof( master_reading.temp_reading ) );
    else
       assignEntry( master_reading.temp_reading, "BGOOD", sizeof( master_reading.temp_reading ) );


    //Setup the AM2315
    if( !dongle.begin() )
       assignEntry( master_reading.extt_reading, "AFAIL", sizeof( master_reading.extt_reading ) );
    else
       assignEntry( master_reading.extt_reading, "AGOOD", sizeof( master_reading.extt_reading ) );

}