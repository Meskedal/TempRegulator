import gspread
from oauth2client.service_account import ServiceAccountCredentials
import requests
import pprint
from time import sleep, localtime, strftime
import datetime
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
	#print(temp)
	new_temp = ''
	for i in range(0,4):
		new_temp += temp[i]
		if(i == 1):
			new_temp += ','
	return new_temp

def insert_temp(temp, sheet, row_index):
	time_now = strftime("%d-%m-%Y %H:%M:%S", localtime())
	time_formatted = "%s.%s.%s kl. %s.%s.%s" % ( time_now[0:2], time_now[3:5], time_now[6:10], time_now[11:13], time_now[14:16],time_now[17:19])	
	sheet.update_cell(row_index,1,time_formatted);
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