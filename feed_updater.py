from oauth2client.service_account import ServiceAccountCredentials
import gspread
import requests
from time import sleep, localtime, strftime

#import pprint
#may need ssl? open ssl?

def get_temp():
	sucess = True
	try:
		response = requests.get('http://192.168.1.99', timeout = 5)
	except (requests.exceptions.ConnectionError):
		print("Conncection error\n")
		sucess = False
	except (requests.exceptions.Timeout):
		print("ESP8266 did not respond\n")
		sucess = False
	#else:
	#	print("Something went wrong\n")
	#	valid = False
	if(sucess):
		return response.text
	else:
		return "error"

def format_temp(temp):
	temp_formated = "%s,%s" @ (temp[0:2],temp[2:4])
	return temp_formated

def insert_temp(temp, sheet, row_index):
	time_now = strftime("%d.%m.%Y kl. %H.%M.%S", localtime())	
	sheet.update_cell(row_index,1,time_now);
	sheet.update_cell(row_index,2,temp)
	

def main():
	row_index = 2
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
	#sheet = gc.open('temperatures').sheet1
	
	print(sheet.row_count)
	while(1):
		sleep(5)
		if credentials.access_token_expired:
			gc.login()
		temp = get_temp()
		if (temp != "error"):
			temp = format_temp(temp)
			insert_temp(temp, sheet, row_index)
			row_index += 1
		#print(temp)
			

if __name__ == "__main__":
    main()


#sheet.update_cell(2,2,'20,3');

#pp = pprint.PrettyPrinter()
#pp.pprint(temperatures)
#temperatures = sheet.get_all_records()