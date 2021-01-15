import redis

r = redis.Redis()
p = r.pubsub()
p.subscribe('First channel', 'Second channel')
r.publish('First channel', 'some info')
r.publish('Second channel', 'some info')
print(p.get_message())
print(p.get_message())
print(p.get_message())
print(p.get_message())
print(p.get_message())
