package unisocial;

public final class TwitterAccount extends SocialMediaAccount {
    public TwitterAccount(String accountId, String username) {
        super(accountId, username);
    }

    @Override
    public String getPlatformName() {
        return "Twitter";
    }

    @Override
    protected void publishPost(Post post) {
        System.out.printf(
            "[%s] @%s posted to Twitter: %s%n",
            post.getCreatedAt(),
            getUsername(),
            post.getContent()
        );
    }
}
