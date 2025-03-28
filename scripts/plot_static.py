import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv('../data/overhead_static.csv')

plt.figure(figsize=(10, 6))
sns.set_theme(style="whitegrid")
plt.rcParams.update({'font.size': 12})

ax = sns.lineplot(
    data=data,
    x='size',
    y='overhead',
    marker='o',
    markersize=8,
    linewidth=2,
    color='#1f77b4'
)

plt.xscale('log')
plt.xticks(data['size'], labels=data['size'])
plt.xlabel('BTB Size', fontweight='bold')
plt.ylabel('Overhead (Instructions fetched / Instructions executed)', fontweight='bold')
# plt.title('BTB Size vs Overhead', pad=20, fontsize=14, fontweight='bold')

for x, y in zip(data['size'], data['overhead']):
    plt.annotate(
        f"{y:.1f}%",
        (x, y),
        textcoords="offset points",
        xytext=(0,10),
        ha='center',
        fontsize=10
    )

plt.ylim(data['overhead'].min()-5, data['overhead'].max()+5)
plt.tight_layout()

plt.savefig('btb_overhead.png', dpi=300, bbox_inches='tight')
plt.show()