import os
import requests
import openai

# GitHub 설정
GITHUB_TOKEN = os.getenv('GITHUB_TOKEN')
REPO_OWNER = 'repo_owner'
REPO_NAME = 'repo_name'
PR_NUMBER = os.getenv('PR_NUMBER')  # pull_request 이벤트의 경우 사용

# OpenAI 설정
OPENAI_API_KEY = os.getenv('OPENAI_API_KEY')

def get_changed_files(repo_owner, repo_name, pr_number, token):
    url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/pulls/{pr_number}/files"
    headers = {'Authorization': f'token {token}'}
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    files = response.json()
    return [file['filename'] for file in files]

def get_file_content(repo_owner, repo_name, file_path, token):
    url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/contents/{file_path}"
    headers = {'Authorization': f'token {token}'}
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    file_content = response.json()['content']
    return file_content

def get_code_review_from_chatgpt(file_content, openai_api_key):
    openai.api_key = openai_api_key
    response = openai.Completion.create(
        engine="davinci-codex",
        prompt=f"Here is a piece of code:\n\n{file_content}\n\nPlease provide a detailed code review:",
        max_tokens=500
    )
    return response.choices[0].text

def create_pr_comment(repo_owner, repo_name, pr_number, body, token):
    url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/issues/{pr_number}/comments"
    headers = {'Authorization': f'token {token}'}
    data = {'body': body}
    response = requests.post(url, json=data, headers=headers)
    response.raise_for_status()
    return response.json()

# Pull Request에서 변경된 파일 가져오기
changed_files = get_changed_files(REPO_OWNER, REPO_NAME, PR_NUMBER, GITHUB_TOKEN)

# 각 파일에 대해 코드 리뷰 요청 및 PR 코멘트 생성
for file_path in changed_files:
    file_content = get_file_content(REPO_OWNER, REPO_NAME, file_path, GITHUB_TOKEN)
    review = get_code_review_from_chatgpt(file_content, OPENAI_API_KEY)
    create_pr_comment(REPO_OWNER, REPO_NAME, PR_NUMBER, review, GITHUB_TOKEN)
