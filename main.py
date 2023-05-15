import pyttsx3
import speech_recognition as sr
import urllib.request
import http.client

# ___________________________________________________________________________________________________

engine = pyttsx3.init()
rate = engine.getProperty('rate')           # getting details of current speaking rate
engine.setProperty('rate', 125)             # setting up new voice rate
volume = engine.getProperty('volume')       # getting to know current volume level (min=0 and max=1)
engine.setProperty('volume', 1.0)           # setting up volume level  between 0 and 1
voices = engine.getProperty('voices')       # getting details of current voice
engine.setProperty('voice', voices[1].id)   # changing index, changes voices. 0 for female, 1 for female

# ___________________________________________________________________________________________________

r = sr.Recognizer()

# ___________________________________________________________________________________________________

main_url = "http://XXX.XXX.XXX.XXX"         #you can get the IP address from the NodeMCU's serial monitor


def sendRequest(url):
    try:
        a = urllib.request.urlopen(url)
    except http.client.RemoteDisconnected:
        print("")


def led1ON():
    sendRequest(main_url + "/led1on")


def led2ON():
    sendRequest(main_url + "/led2on")


def led3ON():
    sendRequest(main_url + "/led3on")


def ledAllON():
    sendRequest(main_url + "/ledallon")


def led1OFF():
    sendRequest(main_url + "/led1off")


def led2OFF():
    sendRequest(main_url + "/led2off")


def led3OFF():
    sendRequest(main_url + "/led3off")


def ledAllOFF():
    sendRequest(main_url + "/ledalloff")


def openDoor():
    sendRequest(main_url + "/opendoor")


def closeDoor():
    sendRequest(main_url + "/closedoor")

# ___________________________________________________________________________________________________

with sr.Microphone(device_index=1) as source:
    while True:
        r.adjust_for_ambient_noise(source)
        audio = r.listen(source, phrase_time_limit=3)
        try:
            words = r.recognize_google(audio)
            if "turn on first light" in words:
                engine.say("first light is turned on")
                engine.runAndWait()
                led1ON()
            elif "turn on second light" in words:
                engine.say("second light is turned on")
                engine.runAndWait()
                led2ON()
            elif "turn on third light" in words:
                engine.say("third light is turned on")
                engine.runAndWait()
                led3ON()
            elif "turn on all lights" in words:
                engine.say("all lights are turned on")
                engine.runAndWait()
                ledAllON()
            elif "turn off first light" in words:
                engine.say("first light is turned off")
                engine.runAndWait()
                led1OFF()
            elif "turn off second light" in words:
                engine.say("second light is turned off")
                engine.runAndWait()
                led2OFF()
            elif "turn off third light" in words:
                engine.say("third light is turned off")
                engine.runAndWait()
                led3OFF()
            elif "turn off all lights" in words:
                engine.say("all lights are turned off")
                engine.runAndWait()
                ledAllOFF()
            elif "open door" in words:
                engine.say("door is opened")
                engine.runAndWait()
                openDoor()
            elif "close door" in words:
                engine.say("door is closed")
                engine.runAndWait()
                closeDoor()
        except sr.UnknownValueError:
            print("Could not understand audio")
        except sr.RequestError as e:
            print("Could not request results; {0}".format(e))
        