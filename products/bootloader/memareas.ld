/* Specify the memory areas */
MEMORY
{
    BOOTLOADER_CODE ( rx  )  : ORIGIN = 0x08000000, LENGTH = 0x14000     /* 40 pages */
    SHARED_FLASH    ( rw  )  : ORIGIN = 0x08080000, LENGTH = 0x800       /* 1  page  */
    RAM1_SHARED     ( xrw )  : ORIGIN = 0x20000000, LENGTH = 0x10
    RAM1            ( xrw )  : ORIGIN = 0x20000010, LENGTH = 0x17FF0
    RAM2            ( xrw )  : ORIGIN = 0x10000000, LENGTH = 0x8000
}

/* External symbols */ 
storage_shared_storage_start = ORIGIN( SHARED_FLASH );
storage_shared_storage_end   = ORIGIN( SHARED_FLASH ) + LENGTH( SHARED_FLASH );
