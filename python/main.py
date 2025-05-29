# from window.window import Window
from pathlib import Path
from tkinter import *
from tkinter import ttk

class AutocompleteCombobox(ttk.Combobox):
    def __init__(self, parent, values, **kwargs):
        super().__init__(parent, **kwargs)
        self.values = values
        self.bind("<KeyRelease>", self.on_keyrelease)
        self['values'] = self.values
    
    def on_keyrelease(self, event):
        typed = self.get().lower()
        
        matches = [value for value in self.values if typed in value.lower()]
        self['values'] = matches
        
        if (typed and matches): 
            self.event_generate('<Down>')

class AltsWindow(Toplevel):
    def __init__(self, parent, cities_list, alts_cities):
        super().__init__(parent)

        self.cities_list = cities_list
        self.alts_cities = alts_cities

        self.city = StringVar()

        # конфигурация окна
        self.title("Выбор точки")
        self.geometry("550x300")
        self.config(bg="#008000")
        self.grab_set()

        all_cities_frame = Frame(self, bg="#008000")
        all_cities_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(all_cities_frame, text="Список городов:", bg="#008000").pack(anchor=CENTER)
        self.all_cities_box = AutocompleteCombobox(all_cities_frame, textvariable=self.city, values=self.cities_list)
        self.all_cities_box.pack(fill=X)
        
        self.submit_btn = ttk.Button(self, text="Подтвердить", command=self.add_and_close)
        self.submit_btn.pack(side=LEFT, expand=True, fill=X, padx=6, pady=10)

    def add_and_close(self):
        if (self.city.get() in self.cities_list) and (self.city.get() not in self.alts_cities):
            self.alts_cities.append(self.city.get())

        self.destroy()

class MainWindow(Tk):
    def __init__(self, cities_list):
        super().__init__()

        self.cities_list = cities_list
 
        quickest_text = "Быстрый"
        longest_text = "Длинный"
        shortest_text = "Короткий"

        # конфигурация окна
        self.title("Основное окно")
        self.geometry("600x350")
        self.config(bg="#008000")

        self.src = StringVar()
        self.dest = StringVar()
        self.priority = StringVar(value="--quickest")

        self.limit = StringVar(value=0)
        self.alts = StringVar(value=0)
        self.altf = StringVar(value=1.0)

        self.alts_cities = []

        top_frame = Frame(self, bg="#008000")
        top_frame.pack(pady=(10, 5), padx=8, fill=X)

        src_frame = Frame(top_frame, bg="#008000")
        src_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(src_frame, text="Источник:", bg="#008000").pack(anchor=W)
        self.src_box = AutocompleteCombobox(src_frame, textvariable=self.src, values=self.cities_list)
        self.src_box.pack(fill=X)
        self.src_box.bind("<<ComboboxSelected>>", self.check)
        self.src_box.bind("<KeyRelease>", self.check, add='+')

        dest_frame = Frame(top_frame, bg="#008000")
        dest_frame.pack(side=LEFT, expand=True, fill=X, padx=(8, 8))
        Label(dest_frame, text="Назначение:", bg="#008000").pack(anchor=W)
        self.dest_box = AutocompleteCombobox(dest_frame, textvariable=self.dest, values=self.cities_list)
        self.dest_box.pack(fill=X)
        self.dest_box.bind("<<ComboboxSelected>>", self.check)
        self.dest_box.bind("<KeyRelease>", self.check, add='+')

        priority_frame = Frame(top_frame, bg="#008000")
        priority_frame.pack(side=LEFT, expand=True, fill=X, padx=0)
        Label(priority_frame, text="Приоритет:", bg="#008000").pack(anchor=W)
        self.quickest_btn = ttk.Radiobutton(priority_frame, text=quickest_text, value="--quickest", variable=self.priority)
        self.quickest_btn.pack(side=TOP, expand=True, fill=X)
        self.longest_btn = ttk.Radiobutton(priority_frame, text=longest_text, value="--longest", variable=self.priority)
        self.longest_btn.pack(side=TOP, expand=True, fill=X)
        self.shortest_btn = ttk.Radiobutton(priority_frame, text=shortest_text, value="--shortest", variable=self.priority)
        self.shortest_btn.pack(side=TOP, expand=True, fill=X)

        middle_frame = Frame(self, bg="#008000")
        middle_frame.pack(pady=(10, 15), padx=8, fill=X)
        
        limit_frame = Frame(middle_frame, bg="#008000")
        limit_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 6))
        Label(limit_frame, text="--limit", bg="#008000").pack(anchor=W)
        limit_spinbox = ttk.Spinbox(limit_frame, from_=0.0, to=20.0, state="readonly", textvariable=self.limit)
        limit_spinbox.pack(fill=X)

        alts_frame = Frame(middle_frame, bg="#008000")
        alts_frame.pack(side=LEFT, expand=True, fill=X, padx=(6, 6))
        Label(alts_frame, text="-alts", bg="#008000").pack(anchor=W)
        alts_spinbox = ttk.Spinbox(alts_frame, from_=0.0, to=20.0, state="readonly", textvariable=self.alts)
        alts_spinbox.pack(fill=X)

        altf_frame = Frame(middle_frame, bg="#008000")
        altf_frame.pack(side=LEFT, expand=True, fill=X, padx=(6, 6))
        Label(altf_frame, text="-altf", bg="#008000").pack(anchor=W)
        altf_spinbox = ttk.Spinbox(altf_frame, from_=1.0, to=10.0, increment=0.1, state="readonly", textvariable=self.altf)
        altf_spinbox.pack(fill=X)

        bottom_frame = Frame(self, bg="#008000")
        bottom_frame.pack(pady=(5, 15))
        self.submit_btn = ttk.Button(bottom_frame, text="Подтвердить", state=DISABLED, command=self.close)
        self.submit_btn.pack(side=LEFT, expand=True, fill=X, padx=6, pady=10)
        self.alts_btn = ttk.Button(bottom_frame, text="Добавить точку в маршрут", state=DISABLED, command=self.add_alts)
        self.alts_btn.pack(side=RIGHT, expand=True, fill=X, padx=6, pady=10)

        self.result = None
 
    def check(self, event=None):
        src = self.src.get()
        dest = self.dest.get()
        cities = self.cities_list

        if (src in cities ) and (dest in cities) and (src != dest):
            self.submit_btn.config(state=NORMAL)
            self.alts_btn.config(state=NORMAL)
            return
        
        self.submit_btn.config(state=DISABLED)
        self.alts_btn.config(state=DISABLED)

    def close(self):
        self.result = [self.src.get()]

        for city in self.alts_cities:
            if ((city != self.src.get()) and (city != self.dest.get())):
                self.result.append(city)

        self.result = self.result + [self.dest.get(), self.priority.get(), "--limit", self.limit.get(), "-alts", self.alts.get(), "-altf", self.altf.get()]
        self.destroy()
    
    def add_alts(self):
        AltsWindow(self, self.cities_list, self.alts_cities)

    def get_data(self):
        return self.result

def main():
    repo_path = Path(__file__).parent.parent
    file_path = repo_path / "input"

    cities_set = set()
    with open(file_path, "r") as f:
        for line in f.readlines():
            city1, city2 = line.split()[:2]
            cities_set.add(city1)
            cities_set.add(city2)
    
    cities_list = []
    while cities_set:
        cities_list.append(cities_set.pop())
    
    cities_list.sort()
    root = MainWindow(cities_list)
    root.mainloop()

    print(root.get_data())


if __name__ == "__main__":
    main()