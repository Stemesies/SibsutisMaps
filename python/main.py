# from window.window import Window
from tkinter import *
from tkinter import ttk

class Window(Tk):
    def __init__(self):
        super().__init__()
 
        # конфигурация окна
        self.title("Основное окно")
        self.geometry("250x200")

        self.src = []
 
        self.src_list = ["Test1", "Test2"]

        self.src_box = ttk.Combobox(values=self.src_list, state="readonly")
        self.src_box.pack(anchor=NW, padx=5, pady=5)
        self.src_box.bind("<<ComboboxSelected>>", self.src_selected)
        
 
    def src_selected(self, event):
        self.src = self.src_box.get()

    def get_src(self):
        return self.src

def main():
    root = Window()
    root.mainloop()

    print(root.get_src())


if __name__ == "__main__":
    main()