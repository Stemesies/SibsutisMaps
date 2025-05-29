# from window.window import Window
from tkinter import *
from tkinter import ttk

class Window(Tk):
    def __init__(self):
        super().__init__()
 
        # конфигурация окна
        self.title("Основное окно")
        self.geometry("250x200")

        self.src = StringVar()
        self.dest = StringVar()
 
        self.cities_list = ["Test1", "Test2"]

        self.src_box = ttk.Combobox(self, textvariable=self.src, values=self.cities_list, state="readonly")
        self.src_box.pack(anchor=NW, padx=5, pady=5)
        self.src_box.bind("<<ComboboxSelected>>", self.check)

        self.dest_box = ttk.Combobox(self, textvariable=self.dest, values=self.cities_list, state="readonly")
        self.dest_box.pack(anchor=NW, padx=5, pady=5)
        self.dest_box.bind("<<ComboboxSelected>>", self.check)

        ttk.Button(self, text="Подтвердить", command=self.close).pack(pady=10)

        self.result = None
        
 
    def check(self, event):
        pass

    def close(self):
        self.result = [self.src.get(), self.dest.get()]
        self.destroy()

    def get_data(self):
        return self.result

def main():
    root = Window()
    root.mainloop()

    print(root.get_data())


if __name__ == "__main__":
    main()