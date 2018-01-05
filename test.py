from oauth2client.service_account import ServiceAccountCredentials
import gspread


def main():
	scope = ['https://spreadsheets.google.com/feeds']
	credentials = ServiceAccountCredentials.from_json_keyfile_name('client_secret.json', scope)
	sheet_connected = False
	while(not sheet_connected):
		try:
			gc = gspread.authorize(credentials)
			sheet = gc.open('temperatures').sheet1
			sheet_connected = True
			print("Connected")
		except:
			sheet_connected = False
			print ("No connection to spreadsheet")
			sleep(5)
	#all_records = sheet.all_records
	sheet.delete_row(2)

if __name__ == "__main__":
    main()