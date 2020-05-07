from entry import Entry
from user import User, existing_users
from faker import Faker
from faker.providers import lorem
import requests


class Artifact:
    urls = ["painting", "sculpture", "museum", "art", "vatican"]

    def __init__(self, museum, collections=[], entry=None, choice=0, lock=0, fake=Faker()):
        self.id = -1
        self.museum = museum
        self.collections = collections
        if entry is None:
            fake.add_provider(lorem)
            title_list = [i.capitalize() for i in fake.words(3)]
            title = " ".join(title_list) + " Artifact"
            introduction = fake.text()
            description = "**Description:** " + fake.paragraph(10)
            image = "https://loremflickr.com/600/400/"
            image += self.urls[choice] + "?lock=" + str(lock)
            self.entry = Entry(title, introduction, image, description)
        else:
            self.entry = entry

    def __str__(self):
        return self.entry.name + "," + str(self.id)

    def __dict__(self):
        return {
            "user": self.museum.user.__dict__(),
            "artifact": self.entry.__dict__(),
            "museum": {"id": self.museum.id},
            "collection": [collection.id for collection in self.collections]
        }





