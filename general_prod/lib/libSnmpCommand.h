#ifndef ___LIB_SNMP_COMMAND__
#define ___LIB_SNMP_COMMAND__

//===================== CPU STATISTIC ==================================


// LOAD
#define ONE_MINUTE_LOAD     ".1.3.6.1.4.1.2021.10.1.3.1"
#define FIVE_MINUTE_LOAD    ".1.3.6.1.4.1.2021.10.1.3.2"
#define FIFTEEN_MINUTE_LOAD ".1.3.6.1.4.1.2021.10.1.3.3"

// CPU
#define PERCENTAGE_USER_CPU_TIME    ".1.3.6.1.4.1.2021.11.9.0"
#define RAW_USER_CPU_TIME           ".1.3.6.1.4.1.2021.11.50.0"
#define PERCENTAGE_SYSTEM_CPU_TIME  ".1.3.6.1.4.1.2021.11.10.0"
#define RAW_SYSTEM_CPU_TIME         ".1.3.6.1.4.1.2021.11.52.0"
#define PERCENTAGE_IDLE_CPU_TIME    ".1.3.6.1.4.1.2021.11.11.0"
#define RAW_IDLE_CPU_TIME           ".1.3.6.1.4.1.2021.11.53.0"
#define RAW_NICE_CPU_TIME           ".1.3.6.1.4.1.2021.11.51.0"


//====================  Memory Statistics  =============================
#define TOTAL_SWAP_SIZE      ".1.3.6.1.4.1.2021.4.3.0"
#define AVAILABLE_SWAP_SPACE ".1.3.6.1.4.1.2021.4.4.0"
#define TOTAL_RAM_MACHINE    ".1.3.6.1.4.1.2021.4.5.0"
#define TOTAL_RAM_USAGE      ".1.3.6.1.4.1.2021.4.6.0"
#define TOTAL_RAM_FREE       ".1.3.6.1.4.1.2021.4.11.0"
#define TOTAL_RAM_SHARED     ".1.3.6.1.4.1.2021.4.13.0"
#define TOTAL_RAM_BUFFERED   ".1.3.6.1.4.1.2021.4.14.0"
#define TOTAL_CACHED_MEMORY  ".1.3.6.1.4.1.2021.4.15.0"

 
//====================  Disk Statistics  ===============================
#define PATH_WHERE_DISK_MOUNTED   	   ".1.3.6.1.4.1.2021.9.1.2.1"
#define PATH_DEVICE_FOR_PARTITION      ".1.3.6.1.4.1.2021.9.1.3.1"
#define TOTAL_SIZE_OF_DEVICE           ".1.3.6.1.4.1.2021.9.1.6.1" // Total size of the disk/partion(kBytes)
#define AVALIBLE_SPACE_ON_DEVICE       ".1.3.6.1.4.1.2021.9.1.7.1" // Свободное место на устройстве (Диске)
#define USAGE_SPACE_ON_DEVICE 	       ".1.3.6.1.4.1.2021.9.1.8.1" // Используемое место на диске
#define PERCENTAGE_SPACE_USED_DEVICE   ".1.3.6.1.4.1.2021.9.1.9.1"

//~ В информатике inode (произносится а́йнод или ино́д), индексный 
//~ дескриптор — это структура данных в традиционных для ОС UNIX файловых 
//~ системах (ФС), таких как UFS. В этой структуре хранится метаинформация
//~ о стандартных файлах, каталогах или других объектах файловой системы, 
//~ кроме непосредственно данных и имени.
#define PERCENTAGE_INODES_USED_DEVICE  ".1.3.6.1.4.1.2021.9.1.10.1" // inodes usage on disk 

//====================  SYSTEM UPTIME   ================================
#define SYSTEM_UPTIME ".1.3.6.1.2.1.1.3.0"



#endif
