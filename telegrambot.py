import telebot
import vk_api
import time

group_id = -69201889
start_of_url = 'https://vk.com/pozor.brno?w=wall'
TOKEN = '1744375818:AAGCoXcNIlFqqjySYSCJYXoJs8-gFLpQNVM'
bot = telebot.TeleBot(TOKEN)

vk_session = vk_api.VkApi('+79061570576', 'scullfucker05')
vk_session.auth()
vk = vk_session.get_api()

MY_CHANNEL_ID = '308594684'

id_post = 404457


def post_id_edit(input_wall):
    global id_post
    all_items = input_wall["items"]
    element = all_items[0]
    if id_post == int(element["id"]):
        bot.send_message(chat_id=MY_CHANNEL_ID, text="Новых постов не обнаружено")
        return
    else:
        id_post = int(element["id"])


def infinity_loop():
    input_wall = vk.wall.search(owner_id=group_id, query='ремонт, сломался', count=5)
    all_items = input_wall["items"]
    x = 0
    for element in all_items:
        element = all_items[x]
        if int(element["id"]) > id_post:
            message_from_group = (str(element["text"] + '\n' + start_of_url + (str(element["owner_id"]) + '_'
                                                                               + str(element["id"]))))
            bot.send_message(MY_CHANNEL_ID, message_from_group)
        x += 1
    post_id_edit(input_wall)


if __name__ == '__main__':
    bot.send_message(chat_id=MY_CHANNEL_ID, text="Приветствую мастер, бот запущен.")
    while True:
        infinity_loop()
        time.sleep(60*60*3)
