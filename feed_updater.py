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
def safe_request(url):
	sucess = True
	try:
		response = requests.get(url, timeout = 5)
	except (requests.exceptions.ConnectionError):
		print("Conncection error\n")
		sucess = False
	except (requests.exceptions.Timeout):
		print("Timeout error\n")
		sucess = False
	#else:
	#	print("Something went wrong\n")
	#	valid = False
	if(sucess):
		return response.text
	else:
		return "error"
def format_temp(temp):
	temp_formated = 0
	print (temp)
	if len(temp) == 4:
		temp_formated = "%s,%s" % (temp[0:2],temp[2:4])
	elif (len(temp) == 3):
		temp_formated = "%s,%s" % (temp[0:1],temp[1:3])
	return temp_formated

def insert_temp(temp, temp_ref, sheet):
	time_now = strftime("%d.%m.%Y kl. %H.%M.%S", localtime())
	values = [time_now, temp, temp_ref]
	print(values[2])
	sheet.delete_row(2)
	sheet.append_row(values)
	
def format_ref(ref):
	ref = ref.replace(".", ",")
	ref = ref.replace("f", "")
	return ref

def token_refresh(gc, credentials):
	if credentials.access_token_expired:
			gc.login()

def main():
	scope = ['https://spreadsheets.google.com/feeds']
	credentials = ServiceAccountCredentials.from_json_keyfile_name('client_secret.json', scope)
	temp_url = 'http://192.168.1.99';
	ref_url = 'http://192.168.1.196/temp-get/';
	sheet_connected = False
	while(not sheet_connected):
		try:
			gc = gspread.authorize(credentials)
			sheet = gc.open('temperatures').sheet1
			sheet_connected = True
			print("Connected to spreadsheet")
		except:
			sheet_connected = False
			print ("No connection to spreadsheet")
			sleep(5)
	sheet.resize(241,3) # 240*1 = 4h span
	while(1):
		sleep(10)
		token_refresh(gc, credentials)
		temp = safe_request(temp_url)
		if (temp != "error"):
			token_refresh(gc, credentials)
			temp_ref = safe_request(ref_url)
			temp = format_temp(temp)
			if(temp_ref != "error"):
				temp_ref = format_ref(temp_ref)
				token_refresh(gc, credentials)
				insert_temp(temp, temp_ref, sheet)
				print("new temp entry")
			else:
				token_refresh(gc, credentials)
				insert_temp(temp, "", sheet)
				
				
		#temp_ref = safe_request(ref_url)
		#print(temp)
			

if __name__ == "__main__":
    main()


#sheet.update_cell(2,2,'20,3');

#pp = pprint.PrettyPrinter()
#pp.pprint(temperatures)
#temperatures = sheet.get_all_records()