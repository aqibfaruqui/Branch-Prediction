import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

original_data = pd.DataFrame({
    'size': [8, 16, 32, 64, 128, 256],
    'overhead': [209.945, 204.55, 198.613, 195.872, 190.262, 187.145],
    'version': ['Original'] * 6
})

new_data = pd.DataFrame({
    'size': [8, 16, 32, 64, 128, 256],
    'overhead': [189.373, 173.766, 162.679, 156.736, 146.145, 140.886],
    'version': ['With 2-bit counter'] * 6
})

combined_data = pd.concat([original_data, new_data])

plt.figure(figsize=(10, 6))
sns.set_theme(style="whitegrid")
plt.rcParams.update({'font.size': 12})

ax = sns.lineplot(
    data=combined_data,
    x='size',
    y='overhead',
    hue='version',
    style='version',
    markers=['o', 's'],
    markersize=8,
    linewidth=2,
    palette=['#1f77b4', '#ff7f0e']
)

plt.xscale('log')
plt.xticks(original_data['size'], labels=original_data['size'])
plt.xlabel('BTB Size', fontweight='bold')
plt.ylabel('Overhead (Instructions fetched / Instructions executed)', fontweight='bold')
plt.legend(title='BTB Implementation', title_fontsize='12', fontsize='11')

for version in ['Original', 'With 2-bit counter']:
    subset = combined_data[combined_data['version'] == version]
    for x, y in zip(subset['size'], subset['overhead']):
        plt.annotate(
            f"{y:.1f}%",
            (x, y),
            textcoords="offset points",
            xytext=(0,10),
            ha='center',
            fontsize=10,
            color='#000000'
        )

min_overhead = min(combined_data['overhead']) - 5
max_overhead = max(combined_data['overhead']) + 5
plt.ylim(min_overhead, max_overhead)

plt.tight_layout()
plt.savefig('btb_overhead_dynamic.png', dpi=300, bbox_inches='tight')
plt.show()