from config import *

from tkinter import *
from tkinter import ttk
import subprocess

class AutocompleteCombobox(ttk.Combobox):
    def __init__(self, parent, values, **kwargs):
        super().__init__(parent, **kwargs)
        self.values = values
        self.bind("<KeyRelease>", self.update_list)
        self['values'] = self.values
    
    def update_list(self, event=None):
        typed = self.get().lower()
        
        matches = [value for value in self.values if typed in value.lower()]
        self['values'] = matches
        
        if (typed and matches): 
            self.event_generate('<Down>')

class AltsWindow(Toplevel):
    def __init__(self, parent, cities_list, alts_cities, src, dest):
        super().__init__(parent)

        self.src = src
        self.dest = dest

        self.cities_list = cities_list
        self.alts_cities = alts_cities

        self.add_city = StringVar()
        self.del_city = StringVar()

        # конфигурация окна
        self.title(ALTS_WINDOW_TITLE)
        self.geometry(ALTS_WINDOW_GEOMETRY)
        self.config(bg=ALTS_WINDOW_BACKGROUND)
        self.grab_set()

        main_frame = Frame(self, bg=MAIN_FRAME_BACKGROUND)
        main_frame.pack(pady=(10, 5), padx=8, fill=X)

        add_frame = Frame(main_frame, bg=ADD_FRAME_BACKGROUND)
        add_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(add_frame, text=ADD_FRAME_LABEL_TEXT, bg=ADD_FRAME_LABEL_BACKGROUND).pack(anchor=CENTER)
        self.add_cities_box = AutocompleteCombobox(add_frame, textvariable=self.add_city, values=self.cities_list)
        self.add_cities_box.pack(fill=X)
        self.submit_add_btn = ttk.Button(add_frame, text=SUBMIT_ADD_BTN_TEXT, command=self.add)
        self.submit_add_btn.pack(side=LEFT, expand=True, fill=X, padx=6, pady=10)

        del_frame = Frame(main_frame, bg=DEL_FRAME_BACKGROUND)
        del_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(del_frame, text=DEL_FRAME_LABEL_TEXT, bg=DEL_FRAME_LABEL_BACKGROUND).pack(anchor=CENTER)
        self.del_cities_box = AutocompleteCombobox(del_frame, textvariable=self.del_city, values=self.alts_cities)
        self.del_cities_box.pack(fill=X)
        self.submit_del_btn = ttk.Button(del_frame, text=SUBMIT_DEL_BTN_TEXT, command=self.delete)
        self.submit_del_btn.pack(side=LEFT, expand=True, fill=X, padx=6, pady=10)

    def add(self):
        city = self.add_city.get()
        if (city in self.cities_list) and (city not in self.alts_cities)\
              and (city != self.src) and (city != self.dest):
            self.alts_cities.append(city)
            self.del_cities_box.update_list()
            self.add_city.set("")
    
    def delete(self):
        city = self.del_city.get()
        if city in self.alts_cities:
            self.alts_cities.remove(city)
            self.del_city.set("")
            self.del_cities_box.update_list()

class InfoWindow(Toplevel):
    def __init__(self, parent, text, title, **kwargs):
        super().__init__(parent, **kwargs)
        self.text = text

        # конфигурация окна
        self.title(title)
        self.geometry(INFO_WINDOW_GEOMETRY)
        self.config(bg=INFO_WINDOW_BACKGROUND)
        self.grab_set()
        
        frame = Frame(self)
        frame.pack(pady=(10, 5), padx=8, fill=BOTH, expand=True)

        self.text_widget = Text(frame, bg=TEXT_WIDGET_BACKGROUND, fg=TEXT_WIDGET_FOREGROUND, font=TEXT_WIDGET_FONT, bd=0, highlightthickness=0)
        self.text_widget.pack(anchor=W, expand=True, fill=BOTH)
        self.text_widget.insert('1.0', self.text)
        self.text_widget.config(state=DISABLED)

class MainWindow(Tk):
    def __init__(self, cities_list):
        super().__init__()

        self.cities_list = cities_list

        # конфигурация окна
        self.title(MAIN_WINDOW_TITLE)
        self.geometry(MAIN_WINDOW_GEOMETRY)
        self.config(bg=MAIN_WINDOW_BACKGROUND)

        self.src = StringVar()
        self.dest = StringVar()
        self.priority = StringVar(value=PRIORITY_DEFAULT_VALUE)

        self.limit = StringVar(value=LIMIT_DEFAULT_VALUE)
        self.alts = StringVar(value=ALTS_DEFAULT_VALUE)
        self.altf = StringVar(value=ALTF_DEFAULT_VALUE)

        self.alts_cities = []

        top_frame = Frame(self, bg=TOP_FRAME_BACKGROUND)
        top_frame.pack(pady=(10, 5), padx=8, fill=X)

        src_frame = Frame(top_frame, bg=SRC_FRAME_BACKGROUND)
        src_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(src_frame, text=SRC_FRAME_LABEL_TEXT, bg=SRC_FRAME_LABEL_BACKGROUND).pack(anchor=W)
        self.src_box = AutocompleteCombobox(src_frame, textvariable=self.src, values=self.cities_list)
        self.src_box.pack(fill=X)
        self.src_box.bind("<<ComboboxSelected>>", self.check)
        self.src_box.bind("<KeyRelease>", self.check, add='+')

        dest_frame = Frame(top_frame, bg=DEST_FRAME_BACKGROUND)
        dest_frame.pack(side=LEFT, expand=True, fill=X, padx=(8, 8))
        Label(dest_frame, text=DEST_FRAME_LABEL_TEXT, bg=DEST_FRAME_LABEL_BACKGROUND).pack(anchor=W)
        self.dest_box = AutocompleteCombobox(dest_frame, textvariable=self.dest, values=self.cities_list)
        self.dest_box.pack(fill=X)
        self.dest_box.bind("<<ComboboxSelected>>", self.check)
        self.dest_box.bind("<KeyRelease>", self.check, add='+')

        priority_frame = Frame(top_frame, bg=PRIORITY_FRAME_BACKGROUND)
        priority_frame.pack(side=LEFT, expand=True, fill=X, padx=(8, 0))
        Label(priority_frame, text=PRIORITY_FRAME_LABEL_TEXT, bg=PRIORITY_FRAME_LABEL_BACKGROUND).pack(anchor=W)
        self.quickest_btn = ttk.Radiobutton(priority_frame, text=PRIORITY_QUICKEST_TEXT, value=QUICKEST_BTN_VALUE, variable=self.priority)
        self.quickest_btn.pack(side=TOP, expand=True, fill=X)
        self.longest_btn = ttk.Radiobutton(priority_frame, text=PRIORITY_LONGEST_TEXT, value=LONGEST_BTN_VALUE, variable=self.priority)
        self.longest_btn.pack(side=TOP, expand=True, fill=X)
        self.shortest_btn = ttk.Radiobutton(priority_frame, text=PRIORITY_SHORTEST_TEXT, value=SHORTEST_BTN_VALUE, variable=self.priority)
        self.shortest_btn.pack(side=TOP, expand=True, fill=X)

        middle_frame = Frame(self, bg=MIDDLE_FRAME_BACKGROUND)
        middle_frame.pack(pady=(10, 15), padx=8, fill=X)
        
        limit_frame = Frame(middle_frame, bg=LIMIT_FRAME_BACKGROUND)
        limit_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(limit_frame, text=LIMIT_FRAME_LABEL_TEXT, bg=LIMIT_FRAME_LABEL_BACKGROUND).pack(anchor=W)
        limit_spinbox = ttk.Spinbox(limit_frame, from_=LIMIT_SPINBOX_FROM, to=LIMIT_SPINBOX_TO, state="readonly", textvariable=self.limit)
        limit_spinbox.pack(fill=X)

        alts_frame = Frame(middle_frame, bg=ALTS_FRAME_BACKGROUND)
        alts_frame.pack(side=LEFT, expand=True, fill=X, padx=(6, 6))
        Label(alts_frame, text=ALTS_FRAME_LABEL_TEXT, bg=ALTS_FRAME_LABEL_BACKGROUND).pack(anchor=W)
        alts_spinbox = ttk.Spinbox(alts_frame, from_=ALTS_SPINBOX_FROM, to=ALTS_SPINBOX_TO, state="readonly", textvariable=self.alts)
        alts_spinbox.pack(fill=X)

        altf_frame = Frame(middle_frame, bg=ALTF_FRAME_BACKGROUND)
        altf_frame.pack(side=LEFT, expand=True, fill=X, padx=(6, 6))
        Label(altf_frame, text=ALTF_FRAME_LABEL_TEXT, bg=ALTF_FRAME_LABEL_BACKGROUND).pack(anchor=W)
        altf_spinbox = ttk.Spinbox(altf_frame, from_=ALTF_SPINBOX_FROM, to=ALTF_SPINBOX_TO, increment=ALTF_SPINBOX_INCREMENT, state="readonly", textvariable=self.altf)
        altf_spinbox.pack(fill=X)

        bottom_frame = Frame(self, bg=BOTTOM_FRAME_BACKGROUND)
        bottom_frame.pack(pady=(5, 15))
        self.submit_btn = ttk.Button(bottom_frame, text=SUBMIT_BTN_TEXT, state=DISABLED, command=self.on_submit)
        self.submit_btn.pack(side=LEFT, expand=True, fill=X, padx=6, pady=10)
        self.alts_btn = ttk.Button(bottom_frame, text=ALTS_BTN_TEXT, state=DISABLED, command=self.add_alts)
        self.alts_btn.pack(side=RIGHT, expand=True, fill=X, padx=6, pady=10)

        self.help_btn = ttk.Button(self, text=HELP_BTN_TEXT, command=self.show_help)
        self.help_btn.pack(side=BOTTOM, anchor=W, padx=6, pady=(10, 5))

        self.result = None
 
    def check(self, event=None):
        src = self.src.get()
        dest = self.dest.get()
        cities = self.cities_list

        if (src in cities) and (dest in cities):
            self.submit_btn.config(state=NORMAL)
            self.alts_btn.config(state=NORMAL)
            return
        
        self.submit_btn.config(state=DISABLED)
        self.alts_btn.config(state=DISABLED)

    def on_submit(self):
        self.result = [self.src.get()]

        for city in self.alts_cities:
            if ((city != self.src.get()) and (city != self.dest.get())):
                self.result.append(city)

        self.result = self.result + [self.dest.get(), self.priority.get(), LIMIT_ARG, self.limit.get(), ALTS_ARG, self.alts.get(), ALTF_ARG, self.altf.get()]
        print(*self.result)

        process = subprocess.run(
            [BIN_FILE_PATH, *self.result],
            capture_output=True,
            text=True
        )

        lines = [line for line in process.stdout.split("\n")][1:]
        if (lines and lines[0][:2] != "Не"):
            lines[0] = "Лучший путь" + lines[0][6:]
            result = "\n".join(lines)
        else:
            result = FAILURE_TEXT

        InfoWindow(self, result, RESULT_TITLE)
    
    def add_alts(self):
        self.alts_cities = [city for city in self.alts_cities if (city != self.src.get()) and (city != self.dest.get())]
        AltsWindow(self, self.cities_list, self.alts_cities, self.src.get(), self.dest.get())
    
    def show_help(self):
        process = subprocess.run(
            [BIN_FILE_PATH],
            capture_output=True,
            text=True
        )

        InfoWindow(self, process.stdout, HELP_TITLE)

def main():
    cities_set = set()
    with open(INPUT_FILE_PATH, "r") as f:
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

if __name__ == "__main__":
    main()