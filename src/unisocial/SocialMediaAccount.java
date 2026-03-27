package unisocial;

/**
 * Abstract observer base class.
 */
public abstract class SocialMediaAccount implements AccountObserver {
    private final String accountId;
    private final String username;

    protected SocialMediaAccount(String accountId, String username) {
        this.accountId = accountId;
        this.username = username;
    }

    @Override
    public String getAccountId() {
        return accountId;
    }

    public String getUsername() {
        return username;
    }

    @Override
    public void update(Post post) {
        publishPost(post);
    }

    protected abstract void publishPost(Post post);
}
