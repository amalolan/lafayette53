class Entry:

    def __init__(self, name="entry", introduction="", image="", description=""):
        self.name = name
        self.introduction = introduction
        self.description = description
        self.image = image

    def __str__(self):
        return self.name + ',' + self.introduction + ',' + self.description + ',' + self.image

    def __dict__(self):
        return {
            "name": self.name,
            "introduction": self.introduction,
            "image": self.image,
            "description": self.description
        }
