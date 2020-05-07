from typing import List
from faker import Faker
from user import create_users, write_and_register_users, User
from museum import Museum, existing_museums, write_and_add_museums, get_museum_ids
import random

fake = Faker()
fake.seed_instance(0)

users: List[User] = create_users(fake)
museums = [i for i in existing_museums]

write_and_register_users(users)
random.seed(0)
for user in users:
    museums.append(Museum(user, choice=random.randint(0, 4), lock=random.randint(0, 2000), fake=fake))
write_and_add_museums(museums)

museums = get_museum_ids(museums)

for museum in museums:
    for i in range(random.randint(3, 10)):
        museum.add_collection(choice=random.randint(0, 4), lock=random.randint(0, 2000), fake=fake)
    museum.get_collections_ids()
    for i in range(random.randint(5, 15)):
        museum.add_artifact(random.sample(museum.collections, random.randint(1, len(museum.collections))),
                            choice=random.randint(0, 4),
                            lock=random.randint(0, 2000), fake=fake)
