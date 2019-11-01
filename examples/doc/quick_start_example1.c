/**
 * @file quick_start_example1.c
 * @brief Quick start example that presents how to use libnfc
 */

// To compile this simple example:
// $ gcc -o quick_start_example1 quick_start_example1.c -lnfc
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <nfc/nfc.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
char *convert(const uint8_t a)
{
  char* buffer2;
  int i;
    
  buffer2 = malloc(9);
  if (!buffer2)
    return NULL;

  buffer2[8] = 0;
  for (i = 0; i <= 7; i++)
    buffer2[7 - i] = (((a) >> i) & (0x01)) + '0';


  puts(buffer2);    
  return buffer2;
}

static void
print_hex(const uint8_t *pbtData, const size_t szBytes)
{
  size_t  szPos;
FILE *fp = fopen("id.txt", "w");
char *final_string;

    fputs("wait\n", fp);
  for (szPos = 0; szPos < szBytes; szPos++) {
    printf("%02x  ", pbtData[szPos]);
    final_string = convert(pbtData[szPos]);
    
	const char* a = final_string;
	int num = 0;
	do {
		int b = *a == '1' ? 1 : 0;
		num = (num << 1) | b;
		a++;
	} while (*a);
  
  

    sprintf(final_string, "%02x", num);
    fputs(final_string, fp);
  }
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
//  tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
  //     tm.tm_hour, tm.tm_min, tm.tm_sec
  fputs("\n", fp);
  sprintf(final_string, "%d", tm.tm_year+1900);
    fputs(final_string, fp);
  sprintf(final_string, "%d", tm.tm_mon+1);
    fputs(final_string, fp);
  sprintf(final_string, "%d", tm.tm_mday);
    fputs(final_string, fp);
  sprintf(final_string, "%d", tm.tm_hour);
    fputs(final_string, fp);
  sprintf(final_string, "%d", tm.tm_min);
    fputs(final_string, fp);
  sprintf(final_string, "%d", tm.tm_sec);
    fputs(final_string, fp);
    fputs("\nGATE1", fp);
    fputs("\nin\n", fp);
    fclose(fp);

  printf("\n");
}
int
main(int argc, const char *argv[])
{
  while(1)
  {
    
    while(1) {
    char buffer[5] = {0, };
    
    FILE *fp = fopen("id.txt", "r");    

    fgets(buffer, sizeof(buffer), fp);  
    
    printf("%s\n", buffer);

    fclose(fp); 
    if(!strcmp(buffer, "done")) {
      break;
    }
  }
  nfc_device *pnd;
  nfc_target nt;

  // Allocate only a pointer to nfc_context
  nfc_context *context;

  // Initialize libnfc and set the nfc_context
  nfc_init(&context);
  if (context == NULL) {
    printf("Unable to init libnfc (malloc)\n");
    exit(EXIT_FAILURE);
  }

  // Display libnfc version
  const char *acLibnfcVersion = nfc_version();
  (void)argc;
  printf("%s uses libnfc %s\n", argv[0], acLibnfcVersion);

  // Open, using the first available NFC device which can be in order of selection:
  //   - default device specified using environment variable or
  //   - first specified device in libnfc.conf (/etc/nfc) or
  //   - first specified device in device-configuration directory (/etc/nfc/devices.d) or
  //   - first auto-detected (if feature is not disabled in libnfc.conf) device
  pnd = nfc_open(context, NULL);

  if (pnd == NULL) {
    printf("ERROR: %s\n", "Unable to open NFC device.");
    exit(EXIT_FAILURE);
  }
  // Set opened NFC device to initiator mode
  if (nfc_initiator_init(pnd) < 0) {
    nfc_perror(pnd, "nfc_initiator_init");
    exit(EXIT_FAILURE);
  }

  printf("NFC reader: %s opened\n", nfc_device_get_name(pnd));

  // Poll for a ISO14443A (MIFARE) tag
  const nfc_modulation nmMifare = {
    .nmt = NMT_ISO14443A,
    .nbr = NBR_106,
  };
  
  if (nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt) > 0) {
    printf("The following (NFC) ISO14443A tag was found:\n");
    printf("    ATQA (SENS_RES): ");
    //print_hex(nt.nti.nai.abtAtqa, 2);
    printf("       UID (NFCID%c): ", (nt.nti.nai.abtUid[0] == 0x08 ? '3' : '1'));
    print_hex(nt.nti.nai.abtUid, nt.nti.nai.szUidLen);
    
 //   final_string = convert(nt.nti.nai.abtUid);
    printf("      SAK (SEL_RES): ");
    //print_hex(&nt.nti.nai.btSak, 1);
    
    if (nt.nti.nai.szAtsLen) {
      printf("          ATS (ATR): ");
      //print_hex(nt.nti.nai.abtAts, nt.nti.nai.szAtsLen);
    }
  }
  
  // Close NFC device
  nfc_close(pnd);
  // Release the context
  nfc_exit(context);
}
  exit(EXIT_SUCCESS);

}
