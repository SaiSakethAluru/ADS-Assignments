import pymongo

def create_counts(filename='counts.txt'):
    friends = {}
    followers = {}
    with open(filename) as f:
        for line in f:
            a,b = map(int,line.strip().split())
            friends[a] = friends.get(a,0) + 1
            followers[b] = followers.get(b,0) + 1
    return friends,followers


def updates_friends_and_followers(mycol=None):
    friends,followers = create_counts()
    if mycol is None:
        myclient = pymongo.MongoClient()
        mydb = myclient['twitter']
        mycol = mydb['tutorial']
    updates = []
    for k,v in friends.items():
        query = {'user.id':k}
        update = {'$set':{'user.new_friends_count':v}}
        updates.append((query,update))
    # updates = []
    for k,v in followers.items():
        query = {'user.id':k}
        update = {'$set':{'user.new_followers_count':v}}
        updates.append((query,update))
    
    for q,u in updates:
        mycol.update_one(q,u)

def top_hastags(mycol=None):
    pipeline = [
        {'$match':{'retweeted_status':{'$exists':1}}},
        {'$unwind':'$entities.hashtags'},
        {'$group':{'_id':{'$toLower':'$entities.hashtags.text'},'count':{'$sum':1}}},
        {'$sort':{'count':-1}},
        {'$limit':5}
    ]
    if mycol is None:
        myclient = pymongo.MongoClient()
        mydb = myclient['twitter']
        mycol = mydb['tutorial']
    result = mycol.aggregate(pipeline)
    for i in result:
        print(i)

def main():
    myclient = pymongo.MongoClient()
    mydb = myclient['twitter']
    mycol = mydb['tutorial']
    print('updating friends and followers')
    updates_friends_and_followers(mycol)
    print('top 5 hashtags')
    top_hastags(mycol)

if __name__=='__main__':
    main()