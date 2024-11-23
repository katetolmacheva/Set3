import pandas as pd
import matplotlib.pyplot as plt

# Загрузка данных из CSV-файла
data = pd.read_csv('results.csv')

# Построение графика приближённой площади S_approx в зависимости от N
plt.figure(figsize=(10,6))
plt.plot(data['N'], data['S_approx'], label='S_approx')
plt.axhline(y=0.944517, color='r', linestyle='--', label='S_exact')
plt.title('Приближённая площадь пересечения трех окружностей vs Количество точек N')
plt.xlabel('Количество точек N')
plt.ylabel('Приближённая площадь S_approx')
plt.legend()
plt.grid(True)
plt.savefig('S_approx_vs_N.png')
plt.show()

# Построение графика относительного отклонения в зависимости от N
plt.figure(figsize=(10,6))
plt.plot(data['N'], data['Relative_Error'] * 100, label='Relative Error (%)')
plt.title('Относительное отклонение S_approx от S_exact vs Количество точек N')
plt.xlabel('Количество точек N')
plt.ylabel('Относительное отклонение (%)')
plt.legend()
plt.grid(True)
plt.savefig('Relative_Error_vs_N.png')
plt.show()

