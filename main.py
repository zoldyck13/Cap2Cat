import torch
import torch.nn as nn
import torch.nn.functional as F
import argparse
import os
import sys
import random


class LSTMGenerator(nn.Module):
    def __init__(self, vocab_size, embed_size, hidden_size, num_layers):
        super(LSTMGenerator, self).__init__()
        self.embed = nn.Embedding(vocab_size, embed_size)
        self.lstm = nn.LSTM(embed_size, hidden_size, num_layers, batch_first=True, dropout=0.2)
        self.fc = nn.Linear(hidden_size, vocab_size)

    def forward(self, x, hidden=None):
        out = self.embed(x)
        out, hidden = self.lstm(out, hidden)
        out = self.fc(out)
        return out, hidden

def apply_smart_leet(word):
    leet_map = {'a': ['@', '4'], 'e': ['3'], 'i': ['1', '!'], 'o': ['0'], 's': ['5', '$'], 't': ['7']}
    variants = {word}
    for char, replacements in leet_map.items():
        new_variants = set()
        for v in variants:
            for r in replacements:
                if char in v:
                    new_variants.add(v.replace(char, r))
        variants.update(new_variants)
    return variants

def generate_smart(model, char_to_idx, idx_to_char, full_keyword, num_passwords, max_len, temp, device):

    tokens = full_keyword.split()
    results = set()
    

    num_per_token = max(1, num_passwords // len(tokens))
    
    print(f"[*] Analyzing tokens: {tokens}")
    print(f"[*] Generating patterns for each part...")

    with torch.no_grad():
        for token in tokens:
            attempts = 0
            token_results = 0
            while token_results < num_per_token and attempts < num_per_token * 3:
                current_seq = "^" + token
                hidden = None
                for _ in range(max_len):
                    x = torch.tensor([[char_to_idx[c] for c in current_seq if c in char_to_idx]], dtype=torch.long).to(device)
                    output, hidden = model(x, hidden)
                    logits = output[0, -1, :]
                    probs = F.softmax(logits / temp, dim=-1)
                    next_idx = torch.multinomial(probs, 1).item()
                    next_char = idx_to_char[next_idx]
                    
                    if next_char == '$' or next_char == '#': break
                    current_seq += next_char
                

                clean_pwd = current_seq.replace("^", "").replace(" ", "")
                if clean_pwd not in results:
                    results.add(clean_pwd)
                    token_results += 1
                attempts += 1


        if len(tokens) > 1:
            results.add("".join(tokens))
            for _ in range(min(10, num_passwords)):
                random.shuffle(tokens)
                results.add("".join(tokens))

    return list(results)

def save_and_filter(passwords, output_file, use_leet):
    valid_passwords = set()
    print("[*] Filtering and applying rules...")
    for pwd in passwords:
        pwd = pwd.strip().replace(" ", "") 
        valid_passwords.add(pwd)
        if use_leet:
            valid_passwords.update(apply_smart_leet(pwd))

    final_count = 0
    with open(output_file, "w") as f:
        for p in valid_passwords:
            if 8 <= len(p) <= 63:
                f.write(p + "\n")
                final_count += 1
    return final_count

def main():
    parser = argparse.ArgumentParser(description="Cap2Cat AI Predictor (Optimized)")
    parser.add_argument("--predict-model", action="store_true")
    parser.add_argument("--keyword", type=str, required=True)
    parser.add_argument("--count", type=int, default=100)
    parser.add_argument("--output", type=str, default="predicted_pass.txt")
    parser.add_argument("--temp", type=float, default=1.1) 
    parser.add_argument("--leet", action="store_true")
    
    args = parser.parse_args()
    model_path = "cap2cat_ai_model.pth"
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    if not os.path.exists(model_path):
        print(f"[-] Model {model_path} not found!")
        return

    checkpoint = torch.load(model_path, map_location=device)
    model = LSTMGenerator(checkpoint['vocab_size'], 
                          checkpoint['config']['embed'], 
                          checkpoint['config']['hidden'], 
                          checkpoint['config']['layers']).to(device)
    model.load_state_dict(checkpoint['model_state'])
    model.eval()


    raw_passwords = generate_smart(model, checkpoint['char_to_idx'], checkpoint['idx_to_char'], 
                                   args.keyword, args.count, 20, args.temp, device)

    final_count = save_and_filter(raw_passwords, args.output, args.leet)

    print(f"\n[+] Success! {final_count} unique passwords saved to: {args.output}")

if __name__ == "__main__":
    main()
