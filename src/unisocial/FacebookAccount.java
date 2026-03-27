package unisocial;

public final class FacebookAccount extends SocialMediaAccount {
    public FacebookAccount(String accountId, String username) {
        super(accountId, username);
    }

    @Override
    public String getPlatformName() {
        return "Facebook";
    }

    @Override
    protected void publishPost(Post post) {
        System.out.printf(
                "[Facebook] user=%s accountId=%s posted: %s%n",
                getUsername(),
                getAccountId(),
                post.getContent()
        );
    }
}
