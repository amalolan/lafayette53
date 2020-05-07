from typing import List
from faker import Faker
from faker.providers import profile as pr
import hashlib
import requests


class User:
    default_password = 'lafayette53'

    def __init__(self, fake=Faker()):
        profile = fake.profile()
        self.username = profile['username']
        self.email = profile['mail']
        self.password = hashlib.sha512(self.default_password.encode("utf-8")).hexdigest()

    def set_username_email(self, username, email):
        self.username = username
        self.email = email

    def __str__(self):
        return self.username + ',' + self.password + ',' + self.email + '\n'

    def __dict__(self):
        return {
            "username": self.username,
            "password": self.password,
            "email": self.email
        }


existing_users = [User() for _ in range(5)]
existing_users[0].set_username_email("pfaffmann", "pfaffmann@example.com")
existing_users[1].set_username_email("malo", "vasum@lafayette.edu")
existing_users[2].set_username_email("lekso", "borashvl@lafayette.edu")
existing_users[3].set_username_email("peter", "lih@lafayette.edu")
existing_users[4].set_username_email("sena", "yevenyos@lafayette.edu")


def create_users(fake=Faker()) -> List[User]:
    fake.add_provider(pr)
    users = []

    for user in existing_users:
        users.append(user)

    for i in range(10):
        users.append(User(fake))

    return users


filename = "users.csv"


def write_and_register_users(users: List[User]) -> None:
    f = open(filename, "w+")
    f.write('username,password,email\n')
    for user in users:
        r = requests.post("http://127.0.0.1:5300/request/register",
                          json=user.__dict__())
        print(r.text)
        f.write(user.__str__())
    print("Mock user data written to " + filename)
