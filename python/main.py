# from window.window import Window
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

class Window(Tk):
    def __init__(self):
        super().__init__()
        self.cities_list = ["Stantsionno-Oyashinskiy", "Test2"]
 
        quickest_text = "Быстрый"
        longest_text = "Длинный"
        shortest_text = "Короткий"

        # конфигурация окна
        self.title("Основное окно")
        self.geometry("550x300")

        self.src = StringVar()
        self.dest = StringVar()
        self.priority = StringVar(value="--quickest")

        self.limit = StringVar(value=0)
        self.alts = StringVar(value=0)
        self.altf = StringVar(value=0)

        top_frame = Frame(self)
        top_frame.pack(pady=(10, 5), padx=8, fill=X)

        src_frame = Frame(top_frame)
        src_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(src_frame, text="Источник:").pack(anchor=W)
        self.src_box = AutocompleteCombobox(src_frame, textvariable=self.src, values=self.cities_list)
        self.src_box.pack(fill=X)
        self.src_box.bind("<<ComboboxSelected>>", self.check)
        self.src_box.bind("<KeyRelease>", self.check)

        dest_frame = Frame(top_frame)
        dest_frame.pack(side=LEFT, expand=True, fill=X, padx=(8, 8))
        Label(dest_frame, text="Назначение:").pack(anchor=W)
        self.dest_box = AutocompleteCombobox(dest_frame, textvariable=self.dest, values=self.cities_list)
        self.dest_box.pack(fill=X)
        self.dest_box.bind("<<ComboboxSelected>>", self.check)
        self.dest_box.bind("<KeyRelease>", self.check)

        priority_frame = Frame(top_frame)
        priority_frame.pack(side=LEFT, expand=True, fill=X, padx=0)
        Label(priority_frame, text="Приоритет:").pack(anchor=W)
        self.quickest_btn = ttk.Radiobutton(priority_frame, text=quickest_text, value="--quickest", variable=self.priority)
        self.quickest_btn.pack(side=TOP, expand=True, fill=X)
        self.longest_btn = ttk.Radiobutton(priority_frame, text=longest_text, value="--longest", variable=self.priority)
        self.longest_btn.pack(side=TOP, expand=True, fill=X)
        self.shortest_btn = ttk.Radiobutton(priority_frame, text=shortest_text, value="--shortest", variable=self.priority)
        self.shortest_btn.pack(side=TOP, expand=True, fill=X)

        middle_frame = Frame(self)
        middle_frame.pack(pady=(10, 15), padx=8, fill=X)
        
        limit_frame = Frame(middle_frame)
        limit_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 6))
        Label(limit_frame, text="--limit").pack(anchor=W)
        limit_spinbox = ttk.Spinbox(limit_frame, from_=0.0, to=20.0, state="readonly", textvariable=self.limit)
        limit_spinbox.pack(fill=X)

        alts_frame = Frame(middle_frame)
        alts_frame.pack(side=LEFT, expand=True, fill=X, padx=(6, 6))
        Label(alts_frame, text="-alts").pack(anchor=W)
        alts_spinbox = ttk.Spinbox(alts_frame, from_=0.0, to=20.0, state="readonly", textvariable=self.alts)
        alts_spinbox.pack(fill=X)

        altf_frame = Frame(middle_frame)
        altf_frame.pack(side=LEFT, expand=True, fill=X, padx=(6, 6))
        Label(altf_frame, text="-altf").pack(anchor=W)
        altf_spinbox = ttk.Spinbox(altf_frame, from_=1.0, to=10.0, increment=0.1, state="readonly", textvariable=self.altf)
        altf_spinbox.pack(fill=X)

        bottom_frame = Frame(self)
        bottom_frame.pack(pady=(5, 15))
        self.submit_btn = ttk.Button(bottom_frame, text="Подтвердить", state=DISABLED, command=self.close)
        self.submit_btn.pack(pady=10)

        self.result = None
 
    def check(self, event=None):
        if ((self.src.get() not in self.cities_list)\
             or (self.dest.get() not in self.cities_list)\
                  or (self.src.get() == self.dest.get())):
            self.submit_btn.config(state=DISABLED)
            return
        
        self.submit_btn.config(state=NORMAL)

    def close(self):
        self.result = [self.src.get(), self.dest.get(), self.priority.get(), self.limit.get(), self.alts.get(), self.altf.get()]
        self.destroy()

    def get_data(self):
        return self.result

def main():
    root = Window()
    root.mainloop()

    print(root.get_data())


if __name__ == "__main__":
    main()