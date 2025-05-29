# from window.window import Window
from tkinter import *
from tkinter import ttk

class Window(Tk):
    def __init__(self):
        super().__init__()
 
        quickest_text = "Быстрый"
        longest_text = "Длинный"
        shortest_text = "Короткий"

        # конфигурация окна
        self.title("Основное окно")
        self.geometry("750x200")

        self.src = StringVar()
        self.dest = StringVar()
        self.priority = StringVar()

        self.cities_list = ["Stantsionno-Oyashinskiy", "Test2"]

        top_frame = Frame(self)
        top_frame.pack(pady=(10, 5), padx=10, fill=X)

        src_frame = Frame(top_frame)
        src_frame.pack(side=LEFT, expand=True, fill=X, padx=(0, 8))
        Label(src_frame, text="Источник:").pack(anchor=W)
        self.src_box = ttk.Combobox(src_frame, textvariable=self.src, values=self.cities_list, state="readonly")
        self.src_box.pack(fill=X)
        self.src_box.bind("<<ComboboxSelected>>", self.check)

        dest_frame = Frame(top_frame)
        dest_frame.pack(side=LEFT, expand=True, fill=X, padx=(8, 8))
        Label(dest_frame, text="Назначение:").pack(anchor=W)
        self.dest_box = ttk.Combobox(dest_frame, textvariable=self.dest, values=self.cities_list, state="readonly")
        self.dest_box.pack(fill=X)
        self.dest_box.bind("<<ComboboxSelected>>", self.check)

        priority_frame = Frame(top_frame)
        priority_frame.pack(side=LEFT, expand=True, fill=X, padx=0)
        Label(priority_frame, text="Приоритет:").pack(anchor=W)
        self.quickest_btn = ttk.Radiobutton(priority_frame, text=quickest_text, value="--quickest", variable=self.priority)
        self.quickest_btn.pack(side=TOP, expand=True, fill=X)

        bottom_frame = Frame(self)
        bottom_frame.pack(pady=(10, 15))
        ttk.Button(bottom_frame, text="Подтвердить", command=self.close).pack(pady=10)

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