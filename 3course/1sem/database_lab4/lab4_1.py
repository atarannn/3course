import redis
import redis.client

r = redis.StrictRedis(host='localhost', db=1)

p = r.pipeline()
r.set('i', 28)
r.incr('i')
p.execute()
print(r.get('i'))
